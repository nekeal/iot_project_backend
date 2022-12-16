import json
from datetime import datetime
from typing import Any, Dict, List

from iot.boxes.models import TimeOfDay


class BoxConfiguration(object):
    def __init__(
        self,
        name: str,
        column: str,
        times: List[str],
        days: List[str],
        sound: str,
        light: str,
    ) -> None:
        self.name = name
        self.column = column
        self.times = times
        self.days = days
        self.sound = sound
        self.light = light

    def get_name(self) -> str:
        return self.name

    def get_column(self) -> str:
        return self.column

    def get_times(self) -> List[str]:
        return self.times

    def get_days(self) -> List[str]:
        return self.days

    def get_sound(self) -> str:
        return self.sound

    def get_light(self) -> str:
        return self.light

    def parse_times(self) -> Dict[str, List[str]]:

        clean_hours = []
        for h in self.get_times():
            try:
                time_query = TimeOfDay.objects.get(name=h)
                clean_hours.append(str(time_query.time))
            except TimeOfDay.DoesNotExist:
                clean_hours.append("00:00")

        result = {}
        days = self.get_days()
        if "codziennie" in days:
            result["poniedzialek"] = clean_hours
            result["wtorek"] = clean_hours
            result["sroda"] = clean_hours
            result["czwartek"] = clean_hours
            result["piatek"] = clean_hours
            result["sobota"] = clean_hours
            result["niedziela"] = clean_hours
        else:
            for d in days:
                result[d] = clean_hours

        return result

    def generate_configuration(self) -> Dict[str, Any]:
        result = {
            "drugName": self.get_name(),
            "column": self.get_column(),
            "openTimes": self.parse_times(),
            "light": 1 if self.get_light() == "swiatlo" else 0,
            "sound": 1 if self.get_sound() == "dzwiek" else 0,
            "timestamp": datetime.now().strftime("%m-%d-%Y, %H:%M:%S"),
        }

        return result
