from typing import Any, List

from django.urls import path

from iot.boxes.views import PublishMessageFormView, GetMessageFormView

app_name = "boxes"

urlpatterns: List[Any] = [
    path("messages/publish/", PublishMessageFormView.as_view(), name="publish_message"),
    path("messages/list/", GetMessageFormView.as_view(), name="list_message"),
]
