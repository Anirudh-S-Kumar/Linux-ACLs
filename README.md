# Access Control Lists

### Description of System
* `fake_root` has uid 0, so behaves like root.
* ACLs are structured in the following manner, each file/directory having it's own ACL stored in the Extended Attributes :-
<pre>
├── Owner
├── ACL
   ├── User_1 : Permissions_1
   ├── User_2 : Permissions_2
   .
   .
   .
</pre>
* Owner and User fields store the uids of the respective users
* For any write operation such as `setacl`, `fput` and `create_dir`, if the ACL doesn't exist, then one is created with the owner added as the owner and stored with the file/directory. 

### Assumptions
* All directories are created with 0755 (`rwxr--xr--x`) permissions.
* You can only create a directory at one level, likewise for changing directory.
* `fput` only works if the file already exists

### Usage
* `make` to compile
* Add `bin` to PATH so that the executables can be run from anywhere
```bash
export PATH=$PATH:$(pwd)/bin
```
* Run `preproc.sh` to set the permissions of the executables
* `make clean` to remove executables


### Testing 
* Setting the acl for a file :-
```bash
setacl <path> <user> <permissions>
```
* Getting the ACL for a file :-
```bash
getacl <path>
```
* Changing the directory :-
```bash
acl_cd <path>
```
* Creating a directory :-
```bash
create_dir <path>
```
* Putting contents in a file :-
```bash
fput <path> "<contents>"
```
* Getting contents of a file :-
```bash
fget <path>
```

### Attacks/Bugs/Errors Defended against
* If no ACL exists for a file, then one is created with the owner as the owner of the file, and if the current user is not the owner, then they are not allowed to perform any operations on the file.
* All the permissions are isolated from each other, and one needs the approriate permissions to perform the operation. For example, only users with read permissions can get the ACL of a file, get contents of a file etc.
* To avoid leaving the program in a privileged state, the effective uid is changed back to the original uid after the priviledged operation is performed.

## Part 2: `simple_sudo`
### Description of the system
* We are using the same ACL system as mentioned before, but are also allowing certain users to run certain binaries which are decided via their ACLs.
* `simple_sudo` will check these ACLs, and if the user has the required permissions, then it will run the binary as the owner, else it will exit with an error.

### Assumptions
* All users are able to run `simple_sudo`, it doesn't have it's own ACL.
* All the programs that require ACL (i.e. `fput`, `getacl` etc.) already have their ACLs set up.

### Usage

* `make` to compile
* Add `bin` to PATH so that the executables can be run from anywhere
```bash
export PATH=$PATH:$(pwd)/bin
```
* Run `preproc.sh` to set the permissions of the executables
* `make clean` to remove executables

### Testing
* Running `simple_sudo` :-
```bash
simple_sudo <path_to_binary> <args>
```

### Attacks/Bugs/Errors Defended against
* If the user doesn't have the required permissions to execute the binary, then it will not be executed.
* If the binary doesn't exist, then it will not be executed.
* To prevent privilege escalation, after the binary is executed, the effective uid is changed back to the original uid.
