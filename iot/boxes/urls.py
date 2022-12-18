from typing import Any, List

from django.urls import path, re_path

from iot.boxes.views import (
    AddOrganizerView,
    BoxConfigView,
    MainPageView,
    ProfileView,
    PublishMessageFormView,
    GetMessageFormView,
    RegisterOrganizerView,
    ShareProfileView,
    TimeOfDayView,
    WelcomePageView,
)

app_name = "boxes"

urlpatterns: List[Any] = [
    path("", MainPageView.as_view(), name="main_page"),
    path("messages/publish/", PublishMessageFormView.as_view(), name="publish_message"),
    path("messages/list/", GetMessageFormView.as_view(), name="list_message"),
    re_path(r"konfiguracja/.*", BoxConfigView.as_view(), name="box_config"),
    path("dodaj-organizer/", AddOrganizerView.as_view(), name="add_organizer"),
    path("pory/", TimeOfDayView.as_view(), name="time_of_day"),
    path("witaj/", WelcomePageView.as_view(), name="welcome_page"),
    path("profil/", ProfileView.as_view(), name="profile"),
    path("wspoldziel/", ShareProfileView.as_view(), name="share_profile"),
    path(
        "zarejestruj-organizer/",
        RegisterOrganizerView.as_view(),
        name="register_organizer",
    ),
]
