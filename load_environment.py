Import("env")

import os

dotenv = 'DOTENV' in os.environ and os.environ['DOTENV'] or ""

if(dotenv == ""):
    print("No dotenv environment variable set, reading from dotenv file")
    with open('.env') as f:
        for line in f:
            dotenv += line + "\n"

print("DOTENV:")
print(dotenv)

env_vars = [] # or dict {}
for line in dotenv.splitlines():
    if line.startswith('#') or not line.strip():
        continue
    key, value = line.strip().split('=', 1)
    env_vars.append({'name': key, 'value': value}) # Save to a list
    env.Append(CPPDEFINES=[
        (key, env.StringifyMacro(value)),
    ])

print(env_vars)

# import subprocess
# subprocess.call(["C:\\WINDOWS\\system32\\WindowsPowerShell\\v1.0\\powershell.exe", ". \"./dotenv.ps1\";"])

# Install missed package
# try:
#     import some_package
# except ImportError:
#     env.Execute("$PYTHONEXE -m pip install some_package")
