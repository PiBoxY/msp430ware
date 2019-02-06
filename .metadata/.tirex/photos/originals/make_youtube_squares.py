import sys
import subprocess
import urllib
import shutil

from string import Template

FILES = [("a2S7REB8nKQ", "msp430wareonlinetraining"),
         ("63JXa3pX3XU", "usbdescriptortool"),
         ("wqOtamhusug", "msp-exp430f5529lpgetstarted"),
         ("Asr0fcQ5uKA", "msp-exp430f5529lpoverview"),
         ("agbpXdbvJ-M", "grace2overview"),
         ("537y7hFryv4", "gracegettingstarted")]

YOUTUBE_URL = "http://img.youtube.com/vi/${ID}/maxresdefault.jpg"

def main():

    for youtube_id, filename  in FILES:

        image_url = Template(YOUTUBE_URL).substitute(ID=youtube_id)
        urllib.urlretrieve(image_url, filename + ".jpg")

        subprocess.call(["convert", filename + ".jpg", "-thumbnail", "200x200^",
                         "-gravity", "center", "-extent", "200x200",
                         filename + "_square.png"])

        try:
            shutil.move(filename + "_square.png", "..")
        except shutil.Error:
            print("Cannot move {}, already exists".format(filename))


if __name__ == '__main__':
    main()
