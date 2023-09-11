# Assignment 1
* Name - Anirudh S Kumar
* Roll No. - 2021517
## Part 1: ACLs and Setuid

### Description of System
* `fake_root` doesn't actually have root privileges, but is in sudoers group. To allow changing the effective uid, the `CAP_SETUID` capability is used, allowing us to change to any effective uid.
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
* `fake_root` is in sudoer's group and has `CAP_SETUID` capability
* All directories are created with 0755 (`rwxr--xr--x`) permissions.
* You can only create a directory at one level, likewise for changing directory.
* `fput` only works if the file already exists

### Usage
* `make` to compile
* Add `bin` to PATH so that the executables can be run from anywhere
```bash
export PATH=$PATH:$(pwd)/bin
```
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