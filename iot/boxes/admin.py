from django.contrib import admin

from iot.boxes.models import Item, TimeOfDay


@admin.register(Item)
class ItemModel(admin.ModelAdmin):
    list_display = ("name", "color", "size")


@admin.register(TimeOfDay)
class TimeOfDayAdmin(admin.ModelAdmin):
    list_display = ("name", "time")
