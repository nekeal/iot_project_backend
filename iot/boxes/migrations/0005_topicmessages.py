# Generated by Django 4.1.2 on 2022-12-18 17:33

import datetime
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ("boxes", "0004_organizer_column_e"),
    ]

    operations = [
        migrations.CreateModel(
            name="TopicMessages",
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
                ("qos", models.IntegerField()),
                ("topic", models.TextField()),
                ("fetch_date", models.DateField(default=datetime.date.today)),
                ("message", models.TextField()),
            ],
        ),
    ]