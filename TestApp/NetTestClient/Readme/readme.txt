This is an experimental template to enable debugging using GDB including to remote Linux targets.

Instructions below for Remote and Local GDB Debugging options.

============================
============================
Remote GDB Debugging
============================
Note: Your Linux machine must have:
    SSH access via certificate auth
    GDB
    Optionally compiler tool chain setup if you intend to build


Follow the 3 steps to be able to debug a process running in a remote linux machine.
Step 4 is to enable intellisense.

Step 1: Get a local SSH client:
============================
You may the SSH client of your choice that will be used to securely tunnel Machine Interface commands to a remote GDB executable.

Win32 Port of OpenSSH
https://github.com/PowerShell/Win32-OpenSSH

Note that for PuTTY you need to use the plink.exe as the SSH Executable. 
Minimally you need putty.exe, pscp.exe and plink.exe. 
puttygen is needed for creating your private key for SSH access. PuTTY is available here:
http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html

Note that MinGW/Cygwin based SSH is fine for the SSH Executable. MinGW/Cygwin based scp does not work from the build step in VS.

Step 2: Configure remote build and debugger:
===========================================
To configure your project for remote debugging go to the project properties->debugging tab. 

You can set project properties there that can be used in the NMake build command line. 

The properties available are:
Host Name: $(RemoteHostName)
User Name: $(RemoteUserName)
Private Key: $(PrivateKey)
Secure Shell Executable: $(SecureShellExecutable)
Remote Working Directory: $(RemoteWorkingDirectory)
Remote Executable Name: $(RemoteExecutable)
Remote Executable Arguments: $(RemoteExecutableArguments)
Remote Debugger Executable: $(RemoteDebuggerExecutable)

Here is an example build command line using PuTTY Tools:
pscp.exe -i $(PrivateKey) source.cpp  $(RemoteUserName)@$(RemoteHostName):$(RemoteWorkingDirectory)/source.cpp
plink.exe $(RemoteUserName)@$(RemoteHostName) -i $(PrivateKey) "cd $(WorkingDirectory);g++ -g source.cpp -o $(RemoteExecutable)"

You can customize it as you like. Your source must be built on a remote Linux machine. By invoking pscp from the build command 
line you can copy your source(s) to the remote Linux machine. Using plink you can invoke the 
compiler on the source on the remote Linux machine. The plink command is capable of executing 
any command supported by the remote target.


Step 3: Configure SSH authentication:
====================================
Generate private/public keys for Win 32 OpenSSH
ssh-keygen.exe -t rsa -f yourkey 

The public key will be in a file yourkey.pub

To create a certificate usable for SSH authentication run puttygen and click generate. 
Save the private and public keys. Copy the displayed text of the public key. 

NOTE: At this time we cannot support certificates that have a pass phrase.
 
On your Linux machine you need to add the public key to the ~/.ssh/authorized_keys file as one line. 
You can do this as follows if the file already exists.
nano ~/.ssh/authorized_keys
In the editor this invokes right click on a new line to insert the copied public key text. 
Hit ctrl-x to exit which promts you to save.
 
If the file does not already exist do the following first:
mkdir ~/.ssh
touch ~/.ssh/authorized_keys
chmod 0700 ~/.ssh
chmod 0600 ~/.ssh/authorized_keys

To connect from an SSH client:
ssh.exe -i privatekey user@host 
 
To connect from PuTTY load your saved session then go to Connection, Data and set your user name. Under Connection, 
SSH, Auth and add the private key you saved. Go back to the session page and save it. Now you can 
open a session using keys without a password.

Note: On your first connection you will be prompted to save the host as a trusted connection. 
You must do this via your SSH client before the other tools can connect via cert auth.


Step 4: Enable Intellisense:
===========================
To enable Intellisense add the paths to your dependencies that are external to your project 
on the appropriate fields on the VC++ Directories tab of the project properties.


============================
============================
Local GDB Debugging
============================
To configure your project for local GDB debugging go to the project properties->debugging tab,
chose the drop down selector and change it to Local GDB.

You can set project properties there that can be used in the NMake build command line. 

The properties available are:
Local Working Directory: $(RemoteWorkingDirectory)
Local Executable: $(RemoteExecutable)
Local Executable Arguments: $(RemoteExecutableArguments)
Local Debugger Executable: $(RemoteDebuggerExecutable)
Local Debugger Server Address: $(LocalDebuggerServerAddress)

Here is an example of building on the local Windows machine.
g++ $(LocalWorkingDirectory)\source.cpp -g -o  $(LocalWorkingDirectory)\$(LocalExecutable)

You can customize it as you like.
