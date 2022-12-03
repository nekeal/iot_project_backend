from django.db import models
from django.utils.translation import gettext_lazy as _


class ColorChoices(models.TextChoices):
    RED = "red", _("Red")
    GREEN = "green", _("Green")
    BLUE = "blue", _("Blue")
    WHITE = "white", _("White")
    BLACK = "black", _("Black")
    YELLOW = "yellow", _("Yellow")


class SizeChoices(models.TextChoices):
    SMALL = "small", _("Small")
    MEDIUM = "medium", _("Medium")
    LARGE = "large", _("Large")


class Item(models.Model):
    name = models.CharField(_("Name"), max_length=255)
    color = models.CharField(_("Color"), choices=ColorChoices.choices, max_length=255)
    size = models.CharField(_("Size"), choices=SizeChoices.choices, max_length=255)

    def __str__(self):
        return self.name

class TimeOfDay(models.Model):
    name = models.CharField(_("Name"), max_length=255)
    time = models.TimeField()
