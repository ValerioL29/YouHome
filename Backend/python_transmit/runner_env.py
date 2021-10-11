from Env import Env
from Rgb import Rgb
from Illegal import Illegal

if __name__ == '__main__':
    # Initialization
    env = Env()
    env.set_client([
        'k4c5aa11.cn-hangzhou.emqx.cloud', 
        'websockets', 
        'telecom', 
        'telecom', 
        'env-percept'
    ])

    # Client Running
    env.run()
