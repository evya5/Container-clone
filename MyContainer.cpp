#include "MyContainer.h"

/**
 * Creates necessary directories in the new fs.
 */
void mkdir_pids() {
    int return_val = EXIT_SUCCESS;
    return_val = mkdir(PATH_TO_FS, DIR_PERMISSIONS);
    err_exit(return_val, MKDIR_FUNC);
    return_val = mkdir(PATH_TO_CGROUP, DIR_PERMISSIONS);
    err_exit(return_val, MKDIR_FUNC);
    return_val = mkdir(PATH_TO_PIDS, DIR_PERMISSIONS);
    err_exit(return_val, MKDIR_FUNC);
}

/**
 * Unmounts the 'proc' folder in the container.
 * @param new_filesystem_directory the fs directory given by the user.
 */
void umount_proc(const char *new_filesystem_directory) {
    std::string path_to_proc = PATH_TO_PROC;
    std::string full_path_to_proc_as_str = new_filesystem_directory+path_to_proc;
    const char * full_path_to_proc = full_path_to_proc_as_str.c_str();
    int return_val = umount(full_path_to_proc);
    err_exit(return_val, UMOUNT_FUNC);
}

/**
 * Removes a single file or directory.
 * @param filepath the path to the relevant file
 */
void remove_file(const std::string& filepath) {
    int return_val  = remove(filepath.c_str());
    err_exit(return_val,REMOVE_FUNC);
}

/**
 * Removes all files and directories which were created by the container's configuration method.
 * @param new_filesystem_directory a fs directory given by the user.
 */
void remove_config_files(const char *new_filesystem_directory) {
    std::string pids_path = PATH_TO_PIDS;
    std::string full_path_to_pids = new_filesystem_directory + pids_path;
    remove_file(full_path_to_pids+CGROUP_PROCS_FILENAMME);
    remove_file(full_path_to_pids+PIDS_MAX_FILENAME);
    remove_file(full_path_to_pids+NOTIFY_ON_RELEASE_FILENAME);
    remove_file(full_path_to_pids);
    std::string cgroup_path = PATH_TO_CGROUP;
    std::string full_path_to_cgroup = new_filesystem_directory+cgroup_path;
    remove_file(full_path_to_cgroup);
    std::string fs_path = PATH_TO_FS;
    std::string full_path_to_fs = new_filesystem_directory+fs_path;
    remove_file(full_path_to_fs);

}

/**
 * Writes to a given file the given content.
 * @param filepath the path to the relevant file
 * @param input the content to write in the file
 */
void write_to_file(const std::string& filepath, const char* input) {

    std::ofstream file(filepath.c_str(), std::ofstream::out);
    file << input;
    file.close();

    int return_value = chmod(filepath.c_str(),DIR_PERMISSIONS);
    err_exit(return_value,CHMOD_FUNC);
}

/**
 * Creates and configures the necessary files under 'sys/fs/cgroup/pids'.
 * @param num_processes max number of processes given by the user.
 */
void config_fs_files(const char* num_processes ) {
    const std::string pids_path = PATH_TO_PIDS;
    const std::string pids_max_filename = PIDS_MAX_FILENAME;
    write_to_file(pids_path+pids_max_filename,num_processes);

    int pid = getpid();
    auto pid_as_string = std::to_string(pid);
    const std::string cgroup_procs_filename = CGROUP_PROCS_FILENAMME;
    write_to_file(pids_path+cgroup_procs_filename,pid_as_string.c_str());

    const std::string notify_on_release_filename = NOTIFY_ON_RELEASE_FILENAME;
    write_to_file(pids_path+notify_on_release_filename,NOTIFY_ON_RELEASE_FLAG);
}

/**
 * Configures all settings for the container
 * @param new_hostname A hostname given by the user.
 * @param new_filesystem_directory a fs directory given by the user.
 * @param num_processes max number of processes given by the user.
 */
void config_container(const char *new_hostname, const char *new_filesystem_directory, const char *num_processes) {
    int return_val;

    /** sets the host name to new_hostname **/
    return_val = sethostname(new_hostname, strlen(new_hostname));
    err_exit(return_val,SETHOSTNAME_FUNC);

    /** changes the root directory to new_filesystem_directory **/
    return_val = chroot(new_filesystem_directory);
    err_exit(return_val,CHROOT_FUNC);


    /** changes the process' working directory to the root '/' **/
    return_val = chdir(FS_BASE_DIR);
    err_exit(return_val,CHDIR_FUNC);

    /** mounts the proc folder in the new fs **/
    return_val = mount(PROC_FOLDER_NAME,PATH_TO_PROC,PROC_FOLDER_NAME,0,nullptr);
    err_exit(return_val,MOUNT_FUNC);

    /** creates necessary directories in the new fs **/
    mkdir_pids();

    /** creates and configures the necessary files under 'sys/fs/cgroup/pids' **/
    config_fs_files(num_processes);
}

/**
 * Performs as a container and runs the given program with the given arguments.
 * @param argv
 * @return 0 an success.
 */
int child(void* argv) {
    /** unpacks program arguments **/
    char ** params = (char **) argv;
    const char *new_hostname  = params[1];
    const char *new_filesystem_directory = params[2];
    const char *num_processes  = params[3];
    const char *path_to_program_to_run_within_container = params[4];
    char ** _args = params + 4;
    config_container(new_hostname, new_filesystem_directory, num_processes);
    /** runs the program as a new process inside the container **/
    execvp(path_to_program_to_run_within_container,_args);

    return EXIT_SUCCESS;
}


int main(int argc, char* argv[]) {
    int return_val = EXIT_SUCCESS;
    void* stack = malloc(STACK);
    if (stack == nullptr) {
        exit(EXIT_ERROR);
    }
    return_val = clone( child,
                        (char*)stack + STACK,
                        CLONE_NEWPID | CLONE_NEWNS | CLONE_NEWUTS | SIGCHLD,
                        argv);
    err_exit(return_val,CLONE_FUNC);
    return_val = wait(nullptr);
    err_exit(return_val,WAIT_FUNC);

    const char *new_filesystem_directory = (char*)argv[2];
    remove_config_files(new_filesystem_directory);
    umount_proc(new_filesystem_directory);
    free(stack);
    return EXIT_SUCCESS;
}