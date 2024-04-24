from dataclasses import dataclass
from scipy import signal
import pybullet as p
import time
import numpy as np
import utils
import argparse
from collections import deque


@dataclass
class Point:
    x: float
    y: float


@dataclass
class World:
    """A class to hold the world objects"""

    plate: int
    sphere: int


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--setpoint", type=float, nargs=2, default=(0.0, 0.0))
    parser.add_argument(
        "--kp", type=float, default=0.0
    )  # set these values to tune the PD controller
    parser.add_argument(
        "--kd", type=float, default=0.0
    )  # set these values to tune the PD controller
    parser.add_argument(
        "--noise", action="store_true", help="Add noise to the measurements"
    )
    parser.add_argument(
        "--filtered", action="store_true", help="filter the measurements"
    )
    cmd_args = parser.parse_args()
    print(cmd_args)
    cmd_args.setpoint = Point(*cmd_args.setpoint)
    return cmd_args


def run_controller(kp, kd, setpoint, noise, filtered, world: World):

    def set_plate_angles(theta_x, theta_y):
        p.setJointMotorControl2(
            world.plate,
            1,
            p.POSITION_CONTROL,
            targetPosition=np.clip(theta_x, -0.1, 0.1),
            force=5,
            maxVelocity=2,
        )
        p.setJointMotorControl2(
            world.plate,
            0,
            p.POSITION_CONTROL,
            targetPosition=np.clip(-theta_y, -0.1, 0.1),
            force=5,
            maxVelocity=2,
        )

    # you can set the variables that should stay accross control loop here

    global buffer_x, buffer_y, buffer_x_filtered, buffer_y_filtered, b, a, error_x_prev, error_y_prev
    error_x_prev, error_y_prev = 0, 0
    
    fc = 100.0
    # Set the filter order
    N_ord = 5
    # Set the filter cutoff in Hz
    f_cutoff = 10
    # Compute the normalization factor as a fraction of the Nyquist frequency
    wn = f_cutoff * 2/fc
    
    q_len = N_ord + 1
    
    buffer_x, buffer_y = deque([0]*5, maxlen=q_len), deque([0]*5, maxlen=q_len)
    buffer_x_filtered, buffer_y_filtered = deque([0]*5, maxlen=q_len), deque([0]*5, maxlen=q_len)

    # Get the filter coefficients
    b, a = signal.butter(N_ord, wn, "low", analog=False)

    def pd_controller(x, y, kp, kd, setpoint: Point):
        """Implement a PD controller, you can access the setpoint via setpoint.x and setpoint.y
        the plate is small around 0.1 to 0.2 meters. You will have to calculate the error and change in error and
        use those to calculate the angle to apply to the plate."""

        global error_x_prev, error_y_prev

        # find errors
        error_x = x - setpoint.x
        error_y = y - setpoint.y
        
        derror_x = (error_x- error_x_prev) / 0.01
        derror_y = (error_y - error_y_prev) / 0.01
        
        error_x_prev, error_y_prev = error_x, error_y

        # proportional term
        fp_x = -kp * error_x
        fp_y = -kp * error_y

        # derivative term
        fd_x = -kd * derror_x
        fd_y = -kd * derror_y

        f_x = fp_x + fd_x
        f_y = fp_y + fd_y

        return f_x, f_y

    def filter_val(val, buffer: deque, buffer_filtered: deque):
        """Implement a filter here, you can use scipy.signal.butter to compute the filter coefficients and then scipy.signal.lfilter to apply the filter.
        but we recommend you implement it yourself instead of using lfilter because you'll have to do that on the real system later.
        Take a look at the butterworth example written by Renato for inspiration."""
        global a, b
        buffer.appendleft(val)
        
        res = 0
        
        for i in range(len(buffer)):
            res += b[i] * buffer[i]
        for i in range(1, len(buffer_filtered)):
            res -= a[i] * buffer_filtered[i-1]
    
        buffer_filtered.appendleft(res)
        
        return res

    def every_10ms(i: int, t: float):
        """This function is called every ms and performs the following:
        1. Get the measurement of the position of the ball
        2. Calculate the forces to be applied to the plate
        3. Apply the forces to the plate
        """
        global buffer_x, buffer_y, buffer_x_filtered, buffer_y_filtered
        (x, y, z), orientation = p.getBasePositionAndOrientation(world.sphere)
        if noise:
            x += utils.noise(t)  # the noise added has a frequency between 30 and 50 Hz
            y += utils.noise(
                t, seed=43
            )  # so that the noise on y is different than the one on x

        if filtered:
            x = filter_val(x, buffer_x, buffer_x_filtered)
            y = filter_val(y, buffer_y, buffer_y_filtered)

        (angle_x, angle_y) = pd_controller(x, y, kp, kd, setpoint)
        set_plate_angles(angle_x, angle_y)

        if i % 10 == 0:  # print every 100 ms
            print(
                f"t: {t:.2f}, x: {x:.3f},\ty: {y:.3f},\tax: {angle_x:.3f},\tay: {angle_y:.3f}"
            )

    utils.loop_every(
        0.01, every_10ms
    )  # we run our controller at 100 Hz using a linux alarm signal


def run_simulation(
    initial_ball_position=Point(
        np.random.uniform(-0.2, 0.2), np.random.uniform(-0.2, 0.2)
    )
):
    p.connect(p.GUI)
    p.setAdditionalSearchPath("assets")
    plate = p.loadURDF("plate.urdf")
    sphere = p.createMultiBody(
        0.2,
        p.createCollisionShape(p.GEOM_SPHERE, radius=0.04),
        basePosition=[initial_ball_position.x, initial_ball_position.y, 0.5],
    )

    # zoom to the plate
    p.resetDebugVisualizerCamera(
        cameraDistance=1.0, cameraYaw=0, cameraPitch=-45, cameraTargetPosition=[0, 0, 0]
    )

    p.setJointMotorControl2(
        plate, 0, p.POSITION_CONTROL, targetPosition=0, force=5, maxVelocity=2
    )
    p.setJointMotorControl2(
        plate, 1, p.POSITION_CONTROL, targetPosition=0, force=5, maxVelocity=2
    )

    p.setGravity(0, 0, -9.8)

    # update the simulation at 100 Hz
    p.setTimeStep(0.01)
    p.setRealTimeSimulation(1)
    return World(plate=plate, sphere=sphere)


if __name__ == "__main__":
    cmd_args = parse_args()
    world = run_simulation()
    run_controller(**vars(cmd_args), world=world)
    time.sleep(10000)
