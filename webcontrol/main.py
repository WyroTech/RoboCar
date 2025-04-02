import asyncio
import websockets

async def handle_connection(websocket):
  print(f"Client connected: {websocket.remote_address}")

  try:
      async for message in websocket:
          print(f"Received message from client: {message}")
          # Echo the message back to the client
          await websocket.send(f"Echo: {message}")
  except websockets.ConnectionClosed:
      print(f"Client disconnected: {websocket.remote_address}")
  except Exception as e:
      print(f"Error: {e}")

async def main():
  server = await websockets.serve(handle_connection, "0.0.0.0", 8080)
  print("WebSocket server started on ws://localhost:8080")
  await server.wait_closed()

if __name__ == "__main__":
  asyncio.run(main())