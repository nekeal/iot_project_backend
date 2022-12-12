from django.contrib import admin

from iot.boxes.models import Item, Organizer, TimeOfDay


@admin.register(Item)
class ItemModel(admin.ModelAdmin):
    list_display = ("name", "color", "size")


@admin.register(TimeOfDay)
class TimeOfDayAdmin(admin.ModelAdmin):
    list_display = ("name", "time")


@admin.register(Organizer)
class OrganizerAdmin(admin.ModelAdmin):
    list_display = ("name", "column_a", "column_b", "column_c", "column_d")
