import json
from typing import Any

from django.contrib import messages
from django.contrib.admin import site as admin_site
from django.http import HttpRequest, HttpResponse
from django.views.generic import FormView

from iot.boxes.models import TopicMessages

from iot.boxes.client import BoxMqttClient
from iot.boxes.forms import PublishMessageForm, GetMessageForm


class PublishMessageFormView(FormView):
    form_class = PublishMessageForm
    template_name = "publish_message.html"

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        return {
            **context,
            **admin_site.each_context(self.request),
            "media": self.form_class().media,
        }

    def form_valid(self, form):
        messages.success(self.request, "Successfully published message")
        return self.get(self.request)

    def post(self, request: HttpRequest, *args: Any, **kwargs: Any) -> HttpResponse:
        form = self.get_form()
        if form.is_valid():
            BoxMqttClient().publish(
                form.cleaned_data["topic"], json.dumps(form.cleaned_data["message"])
            )
            return self.form_valid(form)
        else:
            return self.form_invalid(form)

class GetMessageFormView(FormView):
    form_class = GetMessageForm

    template_name = "listen_message.html"

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        return {
            **context,
            **admin_site.each_context(self.request),
            "media": self.form_class().media,
        }

    def form_valid(self, form):
        messages.success(self.request, "Message fetched")
        return self.get(self.request)

    def post(self, request: HttpRequest, *args: Any, **kwargs: Any) -> HttpResponse:
        form = self.get_form()
        if form.is_valid():
            query_result = TopicMessages.objects.filter(topic=form.cleaned_data["topic"]).order_by("-fetch_date")
            parsed_query = [ "date: {}, message: {}".format(result.fetch_date, result.message) for result in query_result ]
            for msg in parsed_query:
                messages.success(self.request, msg)
            return self.form_valid(form)
        else:
            return self.form_invalid(form)
