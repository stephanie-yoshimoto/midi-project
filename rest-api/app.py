import pygame
import mido
import os
from flask import Flask, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

global path


@app.route('/<string:filename>', methods=['GET'])
def get_file_path(filename):
    for dirpath, dirnames, filenames in os.walk('/Users/stephanie/midi-music-files/'):
        if filename in filenames:
            global path
            path = dirpath + '/' + filename
    return path


@app.route('/midi/<string:filename>/play', methods=['GET'])
def play_music(filename):
    temp = filename
    filename = get_file_path(filename)
    try:
        mido.MidiFile(filename)
    except EOFError:
        # no content in file
        return jsonify({'message': 'cannot play'})
    except IndexError:
        # midi file incorrectly formatted
        return jsonify({'message': 'cannot play'})
    except IOError:
        # file does not exist
        return jsonify({'message': 'cannot play'})

    if pygame.mixer.get_init():
        # if play is clicked when song is already playing then stop song and quit initializer
        pygame.mixer.music.stop()
        pygame.mixer.quit()

    freq = 44100  # audio CD quality
    bit_size = -16  # unsigned 16 bit
    channels = 2  # 1 is mono, 2 is stereo
    buffer_size = 1024  # number of samples
    pygame.mixer.init(freq, bit_size, channels, buffer_size)
    pygame.mixer.music.load(filename)
    pygame.mixer.music.play()
    while pygame.mixer.get_busy():
        continue
    return jsonify({'message': temp})


@app.route('/midi/stop', methods=['GET'])
def stop_music():
    if pygame.mixer.get_init() is None:
        return jsonify({'message': 'mixer not initialized'})
    elif pygame.mixer.music.get_busy():
        pygame.mixer.music.stop()
        pygame.mixer.quit()
        return jsonify({'message': 'success'})
    else:
        return jsonify({'message': 'no music playing'})


@app.route('/midi/<string:filename>/times/<float:multiplier>', methods=['PUT'])
def change_event_time(filename, multiplier):
    try:
        mid = mido.MidiFile(get_file_path(filename))
    except EOFError:
        # no content in file
        return 'no changes made'
    except IndexError:
        # midi file incorrectly formatted
        return 'incorrect format'
    except IOError:
        # file does not exist
        return 'file dne'

    for track in mid.tracks:
        for msg in track:
            msg.time *= multiplier
            msg.time = int(msg.time)
    mid.save(path + filename)
    return 'success'


@app.route('/midi/<string:filename>/octaves/<int:num_octaves>/<string:negative>', methods=['PUT'])
def change_octave(filename, num_octaves, negative):
    try:
        mid = mido.MidiFile(get_file_path(filename))
    except EOFError:
        # no content in file
        return 'no changes made'
    except IndexError:
        # midi file incorrectly formatted
        return 'incorrect format'
    except IOError:
        # file does not exist
        return 'file dne'

    if negative == 'negative':
        num_octaves *= -1

    for track in mid.tracks:
        for msg in track:
            if msg.type == 'note_on' or msg.type == 'note_off' or msg.type == 'polytouch':
                octave_increase = msg.note + (12 * num_octaves)
                if 0 <= octave_increase <= 127:
                    msg.note = octave_increase
    mid.save(path + filename)
    return 'success'


@app.route('/midi/<string:filename>/instruments/<int:instrument>', methods=['PUT'])
def change_event_instrument(filename, instrument):
    try:
        mid = mido.MidiFile(get_file_path(filename))
    except EOFError:
        # no content in file
        return 'no changes made'
    except IndexError:
        # midi file incorrectly formatted
        return 'incorrect format'
    except IOError:
        # file does not exist
        return 'file dne'

    for track in mid.tracks:
        for msg in track:
            if msg.type == 'program_change':
                msg.program = instrument
    mid.save(path + filename)
    return 'success'


@app.route('/midi/<string:filename>/notes', methods=['PUT'])
def change_event_note(filename):
    try:
        mid = mido.MidiFile(get_file_path(filename))
    except EOFError:
        # no content in file
        return 'no changes made'
    except IndexError:
        # midi file incorrectly formatted
        return 'incorrect format'
    except IOError:
        # file does not exist
        return 'file dne'

    # build remapping
    lower = [0] * 0x100
    for i in range(1, 0x100, 12):
        lower[i] = i - 1
    for i in range(6, 0x100, 12):
        lower[i] = i + 1

    for track in mid.tracks:
        for msg in track:
            if msg.type == 'note_on' or msg.type == 'note_off' or msg.type == 'polytouch':
                msg.note = lower[msg.note]

    mid.save(path + filename)
    return 'success'


def check_if_playing():
    if pygame.mixer.get_init():
        if pygame.mixer.music.get_busy():
            return 'True'
        else:
            return 'False'
    else:
        return 'False'


@app.route('/midi/<string:filename>/song_info', methods=['GET'])
def range_of_song(filename):
    try:
        mid = mido.MidiFile(get_file_path(filename))
    except EOFError:
        # no content in file
        return jsonify({'low': 0, 'high': 0, 'length': 0, 'playing': check_if_playing()})
    except IndexError:
        # midi file incorrectly formatted
        return jsonify({'low': 0, 'high': 0, 'length': 0, 'playing': check_if_playing()})
    except IOError:
        # file does not exist
        return jsonify({'low': 0, 'high': 0, 'length': 0, 'playing': check_if_playing()})

    lowest_note = 127
    highest_note = 0
    events_length = 0
    original_length = 0
    for track in mid.tracks:
        for msg in track:
            if msg.type == 'note_on' or msg.type == 'note_off' or msg.type == 'polytouch':
                if msg.note < lowest_note:
                    lowest_note = msg.note
                if msg.note > highest_note:
                    highest_note = msg.note

            # regardless of type of event, include the delta time in events_length
            events_length += msg.time

        # for each track, check if total delta times within track are greater than a previous track's delta time
        if events_length > original_length:
            original_length = events_length

        # reset events_length to count for next track
        events_length = 0

    return jsonify({'low': lowest_note, 'high': highest_note, 'length': original_length, 'playing': check_if_playing()})


if __name__ == '__main__':
    app.run()
