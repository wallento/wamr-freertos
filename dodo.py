import os, subprocess
from doit.action import CmdAction

DOIT_CONFIG = {'default_tasks': ['build']}

def task_ensurebuild():
    return {
        'actions': ['mkdir -p build'],
        'uptodate': [False],
    }

def task_configure():
    """Configure build"""
    return {
        'task_dep': ['ensurebuild'],
        'targets': ['build/build.ninja'],
        'actions': [CmdAction('cmake -G Ninja ..', cwd="build")],
    }

def task_build():
    return {
        'targets': ['build/freertos-wamr'],
        'file_dep': ['build/build.ninja'],
        'actions': [CmdAction('ninja', cwd="build")],
    }

def task_simulate():
    return {
        'task_dep': ['build'],
        'actions': [
            ["renode",
                "-e", "include @buildfiles/nucleo_f429zi.resc",
                "-e", f"sysbus LoadELF '{os.getcwd()}/build/freertos-wamr'",
                "-e", "start"
            ]
        ]
    }

def task_board():
    def run_board():
        p = subprocess.Popen(["openocd", "-f", "openocd.cfg"])
        subprocess.run(["gdb-multiarch", "build/freertos-wamr", "--batch", "-ex", "target remote :3333", "-ex", "load", "-ex", "continue &"])
        p.kill()

    return {
        'actions': [run_board]
    }