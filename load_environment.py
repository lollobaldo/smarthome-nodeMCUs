import os

dotenv = 'DOTENV' in os.environ and os.environ['DOTENV'] or ""

if(dotenv == ""):
    # No dotenv environment variable set, reading from dotenv file
    with open('.env') as f:
        dotenv = f.read()

s = ""
for line in dotenv.splitlines():
    if line.startswith('#') or not line.strip():
        continue
    key, value = line.strip().split('=', 1)
    s += f"'-D{key}=\"{value}\"' "

print(s)
