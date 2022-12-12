from django.contrib import admin

from iot.boxes.models import Item


@admin.register(Item)
class ItemModel(admin.ModelAdmin):
    list_display = ("name", "color", "size")
