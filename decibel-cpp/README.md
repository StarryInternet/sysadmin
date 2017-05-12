# decibel-cpp
Project Decibel library for shared C++ code

### Set up environment

```
vagrant box add --name “opscode/fedora21” http://opscode-vm-bento.s3.amazonaws.com/vagrant/virtualbox/opscode_fedora-21_chef-provisionerless.box
vagrant up
```

This can take some time. Once complete, enter the box by issuing:
`vagrant ssh`

The original directory that you entered the box from is located within the vagrant box at `/vagrant/`
and is a shared directory, meaning that any changes made from within the box will reflect in the
directory outside of the box and vice-versa. `cd` into this directory now with:
`cd /vagrant/`

Download googletest and googlemock and unzip them to your third-party/ directory:
```
wget https://googletest.googlecode.com/files/gtest-1.7.0.zip
wget https://googlemock.googlecode.com/files/gmock-1.7.0.zip
unzip gtest-1.7.0.zip -d third-party/
unzip gmock-1.7.0.zip -d third-party/
```

With everything ready for compilation, you can now run the associated unit tests and compile decible-cpp
(assuming all unit tests passed) with:

`make check`
`make`
