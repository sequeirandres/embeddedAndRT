import socket
import struct
import wave

def receive_audio(host='localhost', port=5000, output_filename='received_audio.wav'):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen(1)
        print(f"Server listening on {host}:{port}")

        conn, addr = s.accept()
        with conn:
            print(f"Connected by {addr}")
            
            sampwidth = struct.unpack('<I', conn.recv(4))[0]  # Receive sample width
            framerate = struct.unpack('<I', conn.recv(4))[0]  # Receive frame rate
            
            audio_data = []
            while True:
                data = conn.recv(2)  # Receive 2 bytes at a time (16-bit samples)
                if not data:
                    break
                audio_data.append(struct.unpack('<h', data)[0])
    
    with wave.open(output_filename, 'wb') as wav:
        wav.setnchannels(1)
        wav.setsampwidth(sampwidth)
        wav.setframerate(framerate)
        wav.writeframes(b''.join(struct.pack('<h', sample) for sample in audio_data))

if __name__ == "__main__":
    receive_audio()