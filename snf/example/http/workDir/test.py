#!/usr/bin/env python
import cgi, cgitb
form = cgi.FieldStorage();
name = form.getvalue("name");
print(name);
