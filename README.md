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
   ├── User : Permissions
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
* Setting the acl for a 