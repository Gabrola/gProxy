using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;
using Algorithms;
using System.Windows.Forms;

#pragma warning disable 1591
namespace gProxyAPI
{
    /// <summary>Path finding class</summary>
    public class PathFinder
    {
        private delegate int InspectAStarBlock(int x, int y, int fromx, int fromy, gMap arg);
        private class AStar
        {
            // Fields
            private gMap Argument;
            private List<AStarRec> AStack;
            private int Freedom;
            private Point Goal;
            private int[,] Grid;
            private InspectAStarBlock InspectBlockCallback;
            private static AStarRec2[] OffSets = new AStarRec2[] { new AStarRec2(0, -1, 10), new AStarRec2(-1, 0, 10), new AStarRec2(1, 0, 10), new AStarRec2(0, 1, 10), new AStarRec2(-1, -1, 14), new AStarRec2(1, -1, 14), new AStarRec2(-1, 1, 14), new AStarRec2(1, 1, 14) };
            private List<Point> Path;
            private Point Source;

            // Methods
            public AStar(PathFinder.InspectAStarBlock CallBack, gMap argument)
            {
                this.Argument = argument;
                this.InspectBlockCallback = CallBack;
                this.AStack = new List<AStarRec>();
                this.Path = new List<Point>();
            }

            private bool FindBest(Point pt, ref Point lowest, ref int lowvalue)
            {
                for (int i = 0; i <= this.Freedom; i++)
                {
                    int num = pt.X + OffSets[i].DX;
                    int num2 = pt.Y + OffSets[i].DY;
                    if (((num >= 0) && (num2 >= 0)) && ((num <= (this.MaxX - 1)) && (num2 <= (this.MaxY - 1))))
                    {
                        if ((num == this.Source.X) && (num2 == this.Source.Y))
                        {
                            return true;
                        }
                        if ((this.Grid[num, num2] > 0) && (this.Grid[num, num2] < lowvalue))
                        {
                            lowvalue = this.Grid[num, num2];
                            lowest.X = num;
                            lowest.Y = num2;
                        }
                    }
                }
                return ((lowest.X == this.Source.X) && (lowest.Y == this.Source.Y));
            }

            public void FindPath(Point Source, Point Goal, Size GridSize, bool Diagonals)
            {
                lock (this)
                {
                    this.Source = Source;
                    this.Goal = Goal;
                    this.Path.Clear();
                    this.AStack.Clear();
                    this.Grid = new int[GridSize.Width, GridSize.Height];
                    this.Freedom = Diagonals ? 7 : 3;
                    this.LookForPath();
                    this.OptimizePath();
                }
            }

            private void InspectBlock(int x, int y, ref bool Found, ref bool Searching, ref AStarRec ClosestgPoint)
            {
                int num;
                if ((x == this.Source.X) && (y == this.Source.Y))
                {
                    num = 0;
                }
                else
                {
                    num = this.Grid[x, y];
                }
                for (int i = 0; i <= this.Freedom; i++)
                {
                    int num2 = x + OffSets[i].DX;
                    int num3 = y + OffSets[i].DY;
                    if ((num2 == this.Goal.X) && (num3 == this.Goal.Y))
                    {
                        Found = true;
                        return;
                    }
                    if (((((num2 >= 0) && (num3 >= 0)) && ((num2 < this.MaxX) && (num3 < this.MaxY))) && ((num2 != this.Source.X) || (num3 != this.Source.Y))) && (this.Grid[num2, num3] == 0))
                    {
                        int num5 = this.InspectBlockCallback(num2, num3, x, y, this.Argument);
                        int num4 = (num + OffSets[i].Cost) + num5;
                        switch (num5)
                        {
                            case -1:
                                break;

                            case 0:
                                Found = false;
                                Searching = false;
                                return;

                            default:
                                AStarRec rec;
                                this.Grid[num2, num3] = num4;
                                if ((Math.Abs((int)(this.Goal.X - num2)) + Math.Abs((int)(this.Goal.Y - num3))) < ClosestgPoint.w)
                                {
                                    ClosestgPoint.pt.X = num2;
                                    ClosestgPoint.pt.Y = num3;
                                    ClosestgPoint.w = Math.Abs((int)(this.Goal.X - num2)) + Math.Abs((int)(this.Goal.Y - num3));
                                }
                                rec.pt = new Point();
                                rec.pt.X = num2;
                                rec.pt.Y = num3;
                                rec.w = num4;
                                this.AStack.Add(rec);
                                break;
                        }
                    }
                }
            }

            private void LookForPath()
            {
                bool searching = false;
                bool found = false;
                AStarRec closestgPoint = new AStarRec();
                closestgPoint.w = 2147483647;
                do
                {
                    if (!searching)
                    {
                        this.InspectBlock(this.Source.X, this.Source.Y, ref found, ref searching, ref closestgPoint);
                        searching = true;
                    }
                    else
                    {
                        if (this.AStack.Count == 0)
                        {
                            throw new Exception(string.Format("Could not calculate path from [" + this.Source.X + "," + this.Source.Y + "] to [" + this.Goal.X + "," + this.Goal.Y + "]"));
                        }
                        int index = 0;
                        for (int i = 0; i < this.AStack.Count; i++)
                        {
                            if (this.AStack[i].w < this.AStack[index].w)
                            {
                                index = i;
                            }
                        }
                        int x = this.AStack[index].pt.X;
                        int y = this.AStack[index].pt.Y;
                        int w = this.AStack[index].w;
                        this.AStack.RemoveAt(index);
                        this.InspectBlock(x, y, ref found, ref searching, ref closestgPoint);
                    }
                }
                while (!found);
            }

            private void OptimizePath()
            {
                Point goal = this.Goal;
                int lowvalue = 2147483647;
                while (true)
                {
                    if (this.FindBest(goal, ref goal, ref lowvalue))
                    {
                        break;
                    }
                    this.Path.Add(goal);
                }
            }

            // Properties
            private int MaxX
            {
                get
                {
                    return this.Grid.GetLength(0);
                }
            }

            private int MaxY
            {
                get
                {
                    return this.Grid.GetLength(1);
                }
            }

            public List<Point> PathRoute
            {
                get
                {
                    lock (this)
                    {
                        return this.Path;
                    }
                }
            }

            // Nested Types
            [StructLayout(LayoutKind.Sequential)]
            private struct AStarRec
            {
                public Point pt;
                public int w;
            }

            [StructLayout(LayoutKind.Sequential)]
            private struct AStarRec2
            {
                public int DX;
                public int DY;
                public int Cost;
                public AStarRec2(int dx, int dy, int cost)
                {
                    this.DX = dx;
                    this.DY = dy;
                    this.Cost = cost;
                }
            }
        }

        /// <summary>Find a path from a point on a map to another point</summary>
        /// <returns>Point[] of path</returns>
        public static Point[] GetPath(gMap Map, int X, int Y, int EndX, int EndY)
        {
            try
            {
                if (!Map[EndX, EndY])
                {
                    throw new ArgumentException(string.Format("The end coordinates are invalid ({0}, {1})", EndX, EndY), "EndX, EndY");
                }
                if (!Map[X, Y])
                {
                    throw new ArgumentException(string.Format("The start coordinates are invalid ({0}, {1})", X, Y), "X, Y");
                }
                AStar star = new AStar(delegate(int x, int y, int fromx, int fromy, gMap arg)
                {
                    int num = 1;
                    if (!arg[x, y])
                    {
                        num = -1;
                    }
                    return num;
                }, Map);
                star.FindPath(new Point(X, Y), new Point(EndX, EndY), new Size(Map.Width, Map.Height), true);
                Point[] pointArray = new Point[star.PathRoute.Count];
                for (int i = 0; i < pointArray.Length; i++)
                {
                    pointArray[(pointArray.Length - i) - 1] = star.PathRoute[i];
                }
                return pointArray;
            }
            catch (Exception exp)
            {
                throw exp;
            }
        }

        /// <summary>Find a path from a point on a map to another point</summary>
        /// <returns>Point[] of path</returns>
        public static Point[] GetPathFast(gMap Map, int X, int Y, int EndX, int EndY)
        {
            int NewWidth = (int)Math.Pow(2d, Math.Ceiling(Math.Log(Map.Width) / Math.Log(2d)));
            int NewHeight = (int)Math.Pow(2d, Math.Ceiling(Math.Log(Map.Height) / Math.Log(2d)));

            byte[,] Grid = new byte[NewWidth, NewHeight];
            Grid.Initialize();

            for (int x = 0; x < Map.Width; x++)
            {
                for (int y = 0; y < Map.Height; y++)
                {
                    Grid[x, y] = Convert.ToByte(Map[x, y]);
                }
            }

            PathFinderFast PathFinder = new PathFinderFast(Grid);
            PathFinder.Formula = HeuristicFormula.Manhattan;
            PathFinder.Diagonals = true;
            PathFinder.HeavyDiagonals = false;
            PathFinder.HeuristicEstimate = 2;
            PathFinder.PunishChangeDirection = false;
            PathFinder.TieBreaker = false;
            PathFinder.SearchLimit = 50000;
            PathFinder.DebugProgress = false;
            PathFinder.ReopenCloseNodes = true;
            PathFinder.DebugFoundPath = true;
            List<PathFinderNode> path = PathFinder.FindPath(new Point(X, Y), new Point(EndX, EndY));

            Point[] pointArray = new Point[path.Count];
            for (int i = 0; i < pointArray.Length; i++)
            {
                pointArray[(pointArray.Length - i) - 1] = new Point(path[i].X, path[i].Y);
            }

            return pointArray;
        }
    }
}
