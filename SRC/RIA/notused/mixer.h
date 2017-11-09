// Mixer.h: interface for the CMixer class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_MIXER_H__7369BAA5_012E_467E_AD1E_A383E4474C58__INCLUDED_)
#define AFX_MIXER_H__7369BAA5_012E_467E_AD1E_A383E4474C58__INCLUDED_

class CMixer 
{
public:
    CMixer(const int VolRange = 100);
    virtual ~CMixer();
    typedef enum _MixerDeice{
        SPEAKERS=0,
        WAVEOUT,
        SYNTHESIZER,
        MICROPHONE
    }MixerDeice;
    bool GetMute(MixerDeice dev);   //����豸�Ƿ���
    bool SetMute(MixerDeice dev,bool vol);     //�����豸����
    bool SetVolume(MixerDeice dev,long vol);   //�����豸������
    unsigned GetVolume(MixerDeice dev);    //�õ��豸������dev=0��������1WAVE ,2MIDI ,3 LINE IN
private:
    bool GetVolumeControl(HMIXER hmixer ,long componentType,long ctrlType,MIXERCONTROL* mxc);
    bool SetVolumeValue(HMIXER hmixer ,MIXERCONTROL *mxc, long volume);
    bool SetMuteValue(HMIXER hmixer ,MIXERCONTROL *mxc, bool mute);
    unsigned GetVolumeValue(HMIXER hmixer ,MIXERCONTROL *mxc);
    long GetMuteValue(HMIXER hmixer ,MIXERCONTROL *mxc);

    long m_VolRange ;
};

#endif // !defined(AFX_MIXER_H__7369BAA5_012E_467E_AD1E_A383E4474C58__INCLUDED_)