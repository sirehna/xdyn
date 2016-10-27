#!/bin/sh
ssh -F config default -C "cd /vagrant/code/debug && ninja"
