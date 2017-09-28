#include "stdafx.h"
#include "FEditPlayer.h"
#include "FEditPlayerImpl.h"
#include <QHBoxLayout>

FEditPlayer::FEditPlayer(QWidget *parent)
	: FBaseWidget(parent)
{
	m_pImpl = new FEditPlayerImpl(this);
	//QHBoxLayout *pLayout = new QHBoxLayout(this);
	//pLayout->addWidget(m_pImpl);
	//pLayout->setMargin(0);
}

FEditPlayer::~FEditPlayer()
{

}

bool FEditPlayer::Open(FNLETimeline *timeline)
{
	return m_pImpl->Open(timeline);
}

bool FEditPlayer::Close()
{
	return m_pImpl->Close();
}

void FEditPlayer::Play()
{
	m_pImpl->Play();
}

void FEditPlayer::Stop()
{
	m_pImpl->Stop();
}

void FEditPlayer::Pause()
{
	m_pImpl->Pause();
}

void FEditPlayer::Seek(int frames)
{
	m_pImpl->Seek(frames);
}

void FEditPlayer::SetDuration(int duration)
{
	m_pImpl->SetDuration(duration);
}

void FEditPlayer::Forward()
{
	m_pImpl->Forward();
}

void FEditPlayer::Backward()
{
	m_pImpl->Backward();
}

void FEditPlayer::Restore()
{
	m_pImpl->Restore();
}

int FEditPlayer::Duration()
{
	return m_pImpl->Duration();
}

bool FEditPlayer::HaveSource()
{
	return m_pImpl->HaveSource();
}

int FEditPlayer::CurrentFrame()
{
	return m_pImpl->CurrentFrame();
}

bool FEditPlayer::IsPlaying()
{
	return m_pImpl->IsPlaying();
}

void FEditPlayer::SetRectangleVisible(bool enabled)
{
	m_pImpl->SetRectangleVisible(enabled);
}

void FEditPlayer::resizeEvent(QResizeEvent *)
{
	m_pImpl->setGeometry(rect());
}

