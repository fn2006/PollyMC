# Reimplementation of https://git.sakamoto.pl/domi/curseme/-/blob/meow/getToken.sh in python

import io
import requests
from re import search

# i have no idea why this is ripping the key from 4chan but what works works i guess
token = requests.get("https://arch.b4k.co/vg/thread/388569358").text
token = search("and put .*? ", token).group(0)
token = search("\$.*$", token).group(0).strip()

output = io.open("token", "w")
output.write(token)
output.close()
