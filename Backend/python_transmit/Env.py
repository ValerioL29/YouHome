from Client import Client

class Env(Client):
    def __init__(self) -> None:
        super().__init__()
    
    def msg_generate(self, msg) -> str:
        eleArr = msg.split(",")
        device_id = eleArr[0]

        temp = eleArr[1]
        while len(temp) < 3:
            temp = "0" + temp

        humi = eleArr[2]
        while len(humi) < 3:
            humi = "0" + humi

        lumi = eleArr[3]
        while len(lumi) < 3:
            lumi = "0" + lumi

        res = device_id + temp + humi + lumi

        return res
