import socket
import wave
import struct
import time

def read_wav_file(filename, channel):
    with wave.open(filename, 'rb') as wav:
        n_channels = wav.getnchannels()
        sampwidth = wav.getsampwidth()
        framerate = wav.getframerate()
        n_frames = wav.getnframes()
        print(f"Channels={n_channels}, Sample Widths={sampwidth}, framerate={framerate}, numFrames={n_frames}")
        if channel >= n_channels:
            raise ValueError("Channel index out of range.")

        audio_data = []
        for i in range(n_frames):
            frame = wav.readframes(1)
            data = struct.unpack('<' + 'h' * n_channels, frame)
            audio_data.append(data[channel])

    return audio_data, sampwidth, framerate

def transmit_audio(audio_data, sampwidth, framerate, host='192.168.1.93', port=5000):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        print(f"Connected to server at {host}:{port}")

        s.sendall(struct.pack('<I', sampwidth))  # Send sample width
        s.sendall(struct.pack('<I', framerate))  # Send frame rate

        sample_interval = 1.0 / framerate  # Time between samples
        for sample in audio_data:
            s.sendall(struct.pack('<h', sample))  # Send audio data
            time.sleep(sample_interval)  # Wait to simulate real-time transmission

if __name__ == "__main__":
    filename = 'CantinaBand60.wav'
    channel = 0  # Change this to select a different channel

    audio_data, sampwidth, framerate = read_wav_file(filename, channel)
    transmit_audio(audio_data, sampwidth, framerate)