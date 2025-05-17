#include "AudioPlayer.h"

#include "Sound.h"

#include <QAudioOutput>
#include <QMediaPlayer>

AudioPlayer::AudioPlayer() : player_(new QMediaPlayer(this)), output_(new QAudioOutput(this)) {
    output_->setVolume(0.5f);
    player_->setAudioOutput(output_);
}

void AudioPlayer::PlaySound(const Sound::Type& sound) const {
    player_->setSource(Sound::GetSoundUrl(sound));
    player_->play();
}
