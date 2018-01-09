# Comment:

#Download ubuntu from the docker hub
FROM ubuntu:latest

#Create a folder inside the home folder on the desktop in the dowloaded image
RUN mkdir -p /home/Desktop/cmps-128/server 

RUN ["chmod", "+x", "/home/Desktop/cmps-128/server/"]

#Copy the executalbe from this folder to the folder instide the image.
COPY . /home/Desktop/cmps-128/server

CMD /home/Desktop/cmps-128/server/server
