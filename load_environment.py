Import("env")

import os

dotenv = 'DOTENV' in os.environ and os.environ['DOTENV'] or ""

if(dotenv == ""):
    print("No dotenv environment variable set, reading from dotenv file")
    with open('.env') as f:
        dotenv = f.read()

for line in dotenv.splitlines():
    if line.startswith('#') or not line.strip():
        continue
    key, value = line.strip().split('=', 1)
    print(f"Setting {key}: {value}")
    env.Append(CPPDEFINES=[
        (key, env.StringifyMacro(value)),
    ])


print("Loaded all.")
