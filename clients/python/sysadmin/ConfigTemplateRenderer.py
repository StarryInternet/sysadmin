import json
import os

from jinja2 import Environment
from jinja2 import FileSystemBytecodeCache
from jinja2 import FileSystemLoader

try:
    from sysadmin_helpers import helpers
except ImportError:
    helpers = None


CACHE_DIR = "/tmp/sysadmin_jinja_cache"


class ConfigTemplateRenderer:
    def __init__(self, templatefile):
        self._has_newline = False
        self.templateFile = templatefile
        if not os.path.isdir(CACHE_DIR):
            os.mkdir(CACHE_DIR)
        cache = FileSystemBytecodeCache(CACHE_DIR, "%s.cache")
        templateName = os.path.basename(templatefile)
        loader = FileSystemLoader(os.path.dirname(templatefile))
        self.env = Environment(trim_blocks=True,
                               lstrip_blocks=True,
                               loader=loader,
                               bytecode_cache=cache,
                               keep_trailing_newline=True)
        self.template = self.env.get_template(templateName)

    def renderTemplate(self, args):
        if helpers is None:
            rendered = self.template.render(json=json, map=map, **args)
        else:
            rendered = self.template.render(__helpers=helpers,
                                            json=json,
                                            map=map,
                                            **args)
        if self._has_newline:
            rendered += '\n'
        return rendered
