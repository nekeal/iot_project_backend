from typing import Any, List

from django.urls import path

from iot.boxes.views import (
    AddOrganizerView,
    BoxConfigView,
    MainPageView,
    ProfileView,
    PublishMessageFormView,
    TimeOfDayView,
    WelcomePageView, ShareProfileView,
)

app_name = "boxes"

urlpatterns: List[Any] = [
    path("", MainPageView.as_view(), name="main_page"),
    path("messages/publish/", PublishMessageFormView.as_view(), name="publish_message"),
    path("konfiguracja/", BoxConfigView.as_view(), name="box_config"),
    path("dodaj-organizer/", AddOrganizerView.as_view(), name="add_organizer"),
    path("pory/", TimeOfDayView.as_view(), name="time_of_day"),
    path("witaj/", WelcomePageView.as_view(), name="welcome_page"),
    path("profil/", ProfileView.as_view(), name="profile"),
    path("wspoldziel/", ShareProfileView.as_view(), name="share_profile"),
]
