# Generated by Django 4.1.2 on 2022-11-28 11:23

from django.db import migrations, models

from iot.boxes.models import ColorChoices


class Migration(migrations.Migration):

    initial = True

    dependencies = []

    operations = [
        migrations.CreateModel(
            name="Item",
            fields=[
                (
                    "id",
                    models.BigAutoField(
                        auto_created=True,
                        primary_key=True,
                        serialize=False,
                        verbose_name="ID",
                    ),
                ),
                ("name", models.CharField(max_length=255, verbose_name="Name")),
                (
                    "color",
                    models.CharField(
                        choices=ColorChoices.choices,
                        max_length=255,
                        verbose_name="Color",
                    ),
                ),
                (
                    "size",
                    models.CharField(
                        choices=[
                            ("small", "Small"),
                            ("medium", "Medium"),
                            ("large", "Large"),
                        ],
                        max_length=255,
                        verbose_name="Size",
                    ),
                ),
            ],
        ),
    ]
