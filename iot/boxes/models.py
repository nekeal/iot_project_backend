from django.db import models
from django.utils.translation import gettext_lazy as _
from datetime import date


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


class Organizer(models.Model):
    name = models.CharField(_("Name"), max_length=255)
    serial_number = models.CharField(_("Serial Number"), max_length=255, unique=True)

    column_a = models.JSONField(_("Column A"), default=dict, blank=True)
    column_b = models.JSONField(_("Column B"), default=dict, blank=True)
    column_c = models.JSONField(_("Column C"), default=dict, blank=True)
    column_d = models.JSONField(_("Column D"), default=dict, blank=True)
    column_e = models.JSONField(_("Column E"), default=dict, blank=True)

    owner = models.ForeignKey(
        "accounts.CustomUser", on_delete=models.CASCADE, related_name="owned_boxes"
    )
    users = models.ManyToManyField(
        "accounts.CustomUser", related_name="boxes", blank=True
    )

    def __str__(self):
        return self.name

class TopicMessages(models.Model):
    qos = models.IntegerField()
    topic = models.TextField()
    fetch_date = models.DateField(default=date.today)
    message = models.TextField()
