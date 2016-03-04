import json
from numpy import integer, floating, ndarray, bool_
from abc import ABCMeta, abstractmethod


class Formatter(object):
    """
    Abstract class for formatters
    """
    __metaclass__ = ABCMeta

    @abstractmethod
    def format_data(self, image_name, data):
        pass

class NullFormatter(Formatter):
    def format_data(self, image_name, data):
        return image_name + '%::%' + json.dumps(data, cls=JsonNumpyEncoder)

    def format_start(self):
        return ''

    def format_close(self):
        return ''


class JsonFormatter(Formatter):
    """
    Formats output in json format
    """

    base_indent = 4
    base_indent_str = ' ' * base_indent

    def __init__(self, prettyprint=False):
        Formatter.__init__(self)
        self.line_separator = False
        self.indent = None if prettyprint is False else 4

    def format_start(self):
        return '{\n'

    def format_close(self):
        return '}\n'

    def format_data(self, image_name, data):
        output = ''
        if self.line_separator:
            output = ''.join((output, ',', '\n'))
        self.line_separator = True
        output_tmp = ''.join(('"', image_name, '"', ': ', json.dumps(data, cls=JsonNumpyEncoder, sort_keys=True,
                                                                     indent=self.indent, separators=(',', ': '))))
        output += ''.join([self.base_indent_str + line + '\n' for line in output_tmp.split('\n')])[:-1]
        return output


class XMLFormatter(Formatter):
    """
    Formats output in xml format
    """
    sep = ' '*4

    def __init__(self):
        Formatter.__init__(self)

    def format_start(self):
        return '<Results>\n'

    def format_close(self):
        return '</Results>\n'

    def format_data(self, image_name, data):
        output = ''.join((XMLFormatter.sep, self.open_tag(image_name), '\n'))
        output = ''.join((output, self.format_dict(data, 2)))
        output = ''.join((output, XMLFormatter.sep, self.close_tag(image_name), '\n'))
        return output

    def format_dict(self, data, indent=0):
        """
        Specific function to format dictionaries
        """
        output = ''
        for key, value in data.iteritems():
            output = ''.join((output, XMLFormatter.sep*indent, self.open_tag(key)))
            if isinstance(value, dict):
                output = ''.join((output, '\n', self.format_dict(value, indent + 1), XMLFormatter.sep*indent))
            elif isinstance(value, list) or isinstance(value, ndarray):
                output = ''.join((output, '\n', self.format_list(value, indent + 1), XMLFormatter.sep*indent))
            else:
                output = ''.join((output, str(value)))
            output = ''.join((output, self.close_tag(key), '\n'))
        return output

    def format_list(self, data, indent=0):
        """
        Specific function to format lists
        """
        output = ''
        for i, elem in enumerate(data):
            output = ''.join((output, XMLFormatter.sep*indent, '<value', str(i),
                              '>', str(elem), '</value', str(i), '>\n'))
        return output

    def open_tag(self, tag):
        return ''.join(('<', tag, '>'))

    def close_tag(self, tag):
        return ''.join(('</', tag, '>'))


class JsonNumpyEncoder(json.JSONEncoder):
    """
    Json custom encoder to handle numpy types
    """
    def default(self, obj):
        if isinstance(obj, integer):
            return int(obj)
        elif isinstance(obj, floating):
            return float(obj)
        elif isinstance(obj, ndarray):
            return obj.tolist()
        elif isinstance(obj, bool_):
            return str(obj)
        else:
            return super(JsonNumpyEncoder, self).default(obj)

