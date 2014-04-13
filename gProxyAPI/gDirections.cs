using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

#pragma warning disable 1591
namespace gProxyAPI
{
    public class gDirections
    {
        public static gDirection North
        {
            get
            {
                return new gDirection(0, 1, 0);
            }
        }

        public static gDirection NorthWest
        {
            get
            {
                return new gDirection(-1, 1, 1);
            }
        }

        public static gDirection West
        {
            get
            {
                return new gDirection(-1, 0, 2);
            }
        }


        public static gDirection SouthWest
        {
            get
            {
                return new gDirection(-1, -1, 3);
            }
        }

        public static gDirection South
        {
            get
            {
                return new gDirection(0, -1, 4);
            }
        }

        public static gDirection SouthEast
        {
            get
            {
                return new gDirection(1, -1, 5);
            }
        }

        public static gDirection East
        {
            get
            {
                return new gDirection(1, 0, 6);
            }
        }

        public static gDirection NorthEast
        {
            get
            {
                return new gDirection(1, 1, 7);
            }
        }

        public static gDirection GetDirection(int Number)
        {
            switch (Number % 8)
            {
                case 0:
                    {
                        return North;
                    }
                case 1:
                    {
                        return NorthWest;
                    }
                case 2:
                    {
                        return West;
                    }
                case 3:
                    {
                        return SouthWest;
                    }
                case 4:
                    {
                        return South;
                    }
                case 5:
                    {
                        return SouthEast;
                    }
                case 6:
                    {
                        return East;
                    }
                case 7:
                    {
                        return NorthEast;
                    }
            }
            return null;
        }
    }

    public class gDirection
    {
        public Point Coordinate;
        public int Number;

        public gDirection(int x, int y, int num)
        {
            this.Coordinate = new Point(x, y);
            this.Number = num;
        }

        public string GetName()
        {
            switch (Number)
            {
                case 0:
                    {
                        return "North";
                    }
                case 1:
                    {
                        return "NorthWest";
                    }
                case 2:
                    {
                        return "West";
                    }
                case 3:
                    {
                        return "SouthWest";
                    }
                case 4:
                    {
                        return "South";
                    }
                case 5:
                    {
                        return "SouthEast";
                    }
                case 6:
                    {
                        return "East";
                    }
                case 7:
                    {
                        return "NorthEast";
                    }
            }
            return String.Empty;
        }

        public gDirection GetOpposite()
        {
            switch (Number)
            {
                case 0:
                    {
                        return gDirections.South;
                    }
                case 1:
                    {
                        return gDirections.SouthEast;
                    }
                case 2:
                    {
                        return gDirections.East;
                    }
                case 3:
                    {
                        return gDirections.NorthEast;
                    }
                case 4:
                    {
                        return gDirections.North;
                    }
                case 5:
                    {
                        return gDirections.NorthWest;
                    }
                case 6:
                    {
                        return gDirections.West;
                    }
                case 7:
                    {
                        return gDirections.SouthWest;
                    }
            }
            return null;
        }
    }
}
