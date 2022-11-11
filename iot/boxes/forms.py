from django import forms
from django.contrib.admin import widgets
from django_json_widget.widgets import JSONEditorWidget


class PublishMessageForm(forms.Form):
    topic = forms.CharField(initial="test_queue", widget=widgets.AdminTextInputWidget)
    message = forms.JSONField(initial={}, widget=JSONEditorWidget)
