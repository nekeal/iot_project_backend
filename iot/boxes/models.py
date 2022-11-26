from django.db import models
from datetime import date


class TopicMessages(models.Model):
    qos = models.IntegerField()
    topic = models.TextField()
    fetch_date = models.DateField(default=date.today)
    message = models.TextField()