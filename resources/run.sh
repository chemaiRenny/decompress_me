socat -T60 TCP-LISTEN:5050,reuseaddr,fork EXEC:"python3 -u challenge.py"