#pragma once

class AnimUtils
{
public:
    template <typename T>
     static T * FindAnimNotifies(UAnimSequenceBase *Animation)
    {
        if (!Animation) return nullptr;
    
        const auto AnimEquipNotifyAll=Animation->Notifies;
        for(auto CurrentNEquipNotify:AnimEquipNotifyAll)
        {
            auto Notify=Cast<T>(CurrentNEquipNotify.Notify);
            if(!Notify)continue;
            return Notify;
        }
        return nullptr;
    }

};