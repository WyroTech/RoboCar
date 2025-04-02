# test_client.py
import asyncio
import websockets
import logging

logging.basicConfig(level=logging.DEBUG)


async def test_connection():
    server_ip = "192.168.2.123"  # Replace with your Mac's IP
    uri = f"ws://{server_ip}:81"

    print(f"Attempting to connect to {uri}")
    try:
        async with websockets.connect(uri) as websocket:
            print("Connected to server")
            await websocket.send("Hello from test client!")
            response = await websocket.recv()
            print(f"Received: {response}")
    except Exception as e:
        print(f"Connection failed: {e}")


if __name__ == "__main__":
    asyncio.run(test_connection())