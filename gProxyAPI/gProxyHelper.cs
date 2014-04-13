using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace gProxyAPI
{
    /// <summary>Extra methods and calculations</summary>
    public class gProxyHelper
    {
        /// <summary>Convert an event number to an actual <see cref="System.Type"/></summary>
        public static Type ConvertEventToType(int EvType)
        {
            switch (EvType)
            {
                case 1:
                    return typeof(HpChange);
                case 2:
                    return typeof(MpChange);
                case 3:
                    return typeof(Jump);
                case 4:
                    return typeof(Walk);
                case 5:
                    return typeof(StatusFlagChange);
                case 6:
                    return typeof(EntitySpawn);
                case 7:
                    return typeof(ItemDrop);
                case 8:
                    return typeof(Chat);
                case 9:
                    return typeof(Attack);
                case 10:
                    return typeof(SpellCast);
                default:
                    return null;
            }
        }

        /// <summary>Measure distance between two points</summary>
        public static uint Distance(uint X1, uint Y1, uint X2, uint Y2)
        {
            return (uint)Math.Max(Math.Abs((int)X1 - (int)X2), Math.Abs((int)Y1 - (int)Y2));
        }

        /// <summary>Measure distance between two points</summary>
        public static uint Distance(int X1, int Y1, int X2, int Y2)
        {
            return (uint)Math.Max(Math.Abs(X1 - X2), Math.Abs(Y1 - Y2));
        }

        /// <summary>Draws a map as a bitmap</summary>
        public static Bitmap DrawMap(gMap Map)
        {
            Bitmap Bmp = new Bitmap(Map.Width, Map.Height);
            Graphics gfx = Graphics.FromImage(Bmp);

            for (int x = 0; x < Map.Width; x++)
            {
                for (int y = 0; y < Map.Height; y++)
                {
                    if (Map[x, y])
                        gfx.FillRectangle(new SolidBrush(Color.White), x, y, 1, 1);
                    else
                        gfx.FillRectangle(new SolidBrush(Color.Black), x, y, 1, 1);
                }
            }

            return Bmp;
        }
    }
}
