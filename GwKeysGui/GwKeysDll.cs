using GwKeys;

namespace GwKeys.Gui
{
    public class GwKeysDll
    {
        private CapsToGWBasicAscii dll;
        private bool gwKeysOn;

        public GwKeysDll()
        {
            dll = new CapsToGWBasicAscii();
            gwKeysOn = false;
        }

        public bool On
        {
            get
            {
                return gwKeysOn;
            }
            set
            {
                if (value != gwKeysOn)
                {
                    toggle();
                }                
            }
        }

        public bool toggle()
        {
            if (!gwKeysOn)
            {
                dll.Install();
            }
            else
            {
                dll.Uninstall();
            }
            gwKeysOn = !gwKeysOn;
            return On;
        }
    }

}