target extended-remote localhost:3333

monitor reset halt

file build/os.elf

load

break main


continue