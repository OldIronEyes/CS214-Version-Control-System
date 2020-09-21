# CS214-Version-Control-System

Class: Systems Programming, Spring 2019

Intro: This project required us to write a simple one way version control system that would use client and server programs to send files over the internet and maintain a repo for changed documents. To do so, the programs had to keep track of four commands: Update, Upgrade, Commit, and Push which are detailed below.

Update - Get the server's .Manifest and compare all entries in it with the client's .Manifest and see what changes need to be made to the client's files to bring them up to the same version as theserver, and write out a .Update file recording all those changes that need to be made.
Upgrade - Make all the changes listed in the .Update to the client side
Commit - Get the server's .Manifest and compare all entries in it with the client's .Manifest and find out which files the client has that are newer versions than the ones on the server, or the server does not have, and write out a .Commit recording all the changes that need to be made.
Push - Make all the changes listed in the .Commit to the server side

Full assignment details are available in Asst3.pdf
