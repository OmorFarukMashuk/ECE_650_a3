#!/usr/bin/env python

from __future__ import print_function
from __future__ import division
import sys
import shlex
import cmd
import re
import math

class Iteration(cmd.Cmd):
    prompt = ''
    use_rawinput = 0

    def __init__(self):
        cmd.Cmd.__init__(self, completekey=None)
        self.graph = Graph()

    def parseline(self, ipline):
        """overriding parseline method"""
        ipline = ipline.strip()
        if not ipline:
            return None, None, ipline
        i = 0
        while i < len(ipline) and ipline[i] != ' ':
            i = i+1
        cmd = ipline[:i]
        arg = ipline[i:].strip()

        return cmd, arg, ipline

    def do_a(self, args):
        """
        This is the help string for the add command
        """
        pargs = (DoParsing(args, 'do_a'))
        if pargs:
            self.graph.AddStreet(*pargs)

    def do_r(self, args):
        """
        This is the help string for the remove command
        """
        pargs = (DoParsing(args, 'do_r'))
        if pargs:
            self.graph.RemoveStreet(*pargs)

    def do_c(self, args):
        """
        This is the help string for the change command
        """
        pargs = (DoParsing(args, 'do_c'))
        if pargs:
            self.graph.ChangeStreet(*pargs)

    def do_g(self, args):
        """
        This is the help string for the graph command
        """
        if not args:
            self.graph.generateGraph()
            print(self.graph)
        else:
            print('Error: Invalid arguments')

    def postcmd(self, stop, ipline):
        """Hook method executed just after a command dispatch is finished."""
        #If ipline empty, exit program
        if(not ipline or ipline=='EOF'):
            stop = True
        return stop

    def default(self, ipline):
        if ipline != 'EOF':
            print('Error: Invalid Command')

    def emptyline(self):
        pass

class Graph(object):
    def __init__(self):
        self.past_list = {}
        self.vertex_list = {}
        self.edge_list = set([])
        self.insectlist = set([])

    def __str__(self):

        v_map = {v_id: idx for idx, v_id in enumerate(self.vertex_list.values())}
        string = 'V {0}\n'.format(len(self.vertex_list))
        string += 'E {'
        if self.edge_list:
            for edge in self.edge_list:
                temp = list(edge)
                v_1 = v_map[temp[0]]
                v_2 = v_map[temp[1]]
                string += '<{0},{1}>,'.format(v_1, v_2)
            string = string[:-1] + '}'
        else:
            string += '}'

        return string

    def AddStreet(self, street, vertex_list):

        if vertex_list:
            if street in self.past_list:
                print('Error: Street already exists')
            else:
                self.past_list[street] = vertex_list
                return True
        else:
            print('Error: Invalid Command')

        return False

    def ChangeStreet(self, street, vertex_list):
        if vertex_list:
            if street in self.past_list:
                self.past_list[street] = vertex_list
                return True
            else:
                print('Error: Street does not exist')
        else:
            print('Error: Invalid input')

        return False

    def RemoveStreet(self, street, *args):
        if street in self.past_list:
            del self.past_list[street]
            return True
        else:
            print('Error: Street does not exist')

        return False


    def AddSegmentIntersections(self,i,points,points_2, temp_p_to_add,temp_intersections):
        for j in xrange(len(points_2)-1):
            inter_p = getIntersections(points[i], points[i+1], points_2[j], points_2[j+1])
            if inter_p:
                [temp_intersections.add(x) for x in inter_p]
                [temp_p_to_add.append(x) for x in inter_p if (x != points[i] and x != points[i+1])]
        return temp_p_to_add, temp_intersections

    def AddAdditionalIntersections(self,i,points,temp_p_to_add,temp_intersections,temp_graph,street):
        if (points[i] in temp_intersections
                or points[i+1] in temp_intersections
                or len(temp_p_to_add) > 0
                or (temp_graph[street] or [None])[-1] in temp_intersections):
            temp_graph[street].append(points[i])

        if len(temp_p_to_add) > 1:
            temp_p_to_add = list(set(temp_p_to_add)) # remove duplicates
            temp_dist = [getDistance(p, points[i]) for p in temp_p_to_add]
            temp_dist, temp_p_to_add = zip(*sorted(zip(temp_dist, temp_p_to_add)))
        for temp_p in temp_p_to_add:
            temp_graph[street].append(temp_p)
        return temp_graph, temp_intersections

    def AddEdgeIntersectionWithOtherStreets(self,i,street,points,temp_graph,temp_intersections):
        temp_p_to_add = []
        for street_2, points_2 in self.past_list.iteritems():
            if street != street_2:
                temp_p_to_add, temp_intersections = self.AddSegmentIntersections(i,points,points_2,temp_p_to_add,temp_intersections)

        return self.AddAdditionalIntersections(i,points,temp_p_to_add,temp_intersections,temp_graph,street)

    def AddStreetEdges(self,street,points,temp_graph,temp_intersections):
        for i in xrange(len(points)-1):
            temp_graph, temp_intersections = self.AddEdgeIntersectionWithOtherStreets(i,street,points,temp_graph,temp_intersections)
        if (temp_graph[street] or [None])[-1] in temp_intersections:
            temp_graph[street].append(points[-1])

        return temp_graph, temp_intersections

    def generateGraph(self):
        self.edge_list = set([])
        temp_graph = {}
        temp_intersections = set([])

        for street, points in self.past_list.iteritems():
            temp_graph[street] = []
            temp_graph, temp_intersections = self.AddStreetEdges(street,points,temp_graph,temp_intersections)


        to_remove = set([])
        for _, v in temp_graph.iteritems():
            [to_remove.add(x) for x in v]
        to_remove = set(self.vertex_list.keys()) - temp_intersections
        {self.vertex_list.pop(x) for x in to_remove}
        self.insectlist = temp_intersections

        i = 1
        for street, vertex_list in temp_graph.iteritems():
            prev = None
            for index, vertex in enumerate(vertex_list):
                if vertex not in self.vertex_list:
                    while i in self.vertex_list.values():
                        i +=1
                    self.vertex_list[vertex] = i

                v_id = self.vertex_list[vertex]
                if(index > 0 and (vertex in self.insectlist or prev in self.insectlist) ):
                    self.edge_list.add(frozenset([v_id, self.vertex_list.get(prev)]))
                prev = vertex
        return

def DoParsing(args, func):
    """return a list [street, [list of points]]
        returns False if Error
    """
    if not args:
        print('Error: invalid input')
        return False
    try:
        temp = shlex.split(args)
    except:
        print('Error: Invalid input')
        return False

    street = temp[0].lower()
    if re.search(r'[^A-Za-z0-9 ]', street):
        print('Error: Invalid street name')
        return False

    if len(temp) > 1:
        if func == 'do_r':
            print('Error: r command has invalid arguments')
            return False
        vertex_list = ''.join(temp[1:])
        if re.search(r'[^0-9,\(\)\- ]', vertex_list):
            print('Error: Invalid vertex', file=sys.stderr)
            return False

        #Check all vertex_list have open and closing parentheses
        open_paren_num = vertex_list.count('(')
        close_paren_num = vertex_list.count(')')
        if open_paren_num != close_paren_num:
            print('Error: Invalid format')
            return False

        # match everything between '(' and ')'
        regex = r'\((.*?)\)+?'
        vertex_list = re.findall(regex, vertex_list)
        parsed_vertices = []

        # Cast all inputs to tuples of type int
        try:
            for vertex in vertex_list:
                coords = vertex.split(',')
                if len(coords) != 2:
                    raise Exception
                parsed_vertices.append(tuple([int(x) for x in coords]))
        except:
            print('Error: Invalid input')
            return False

        if (len(parsed_vertices) == 0 or
            len(parsed_vertices) != open_paren_num):

            print('Error: Invalid vertex')
            return False

        return [street, parsed_vertices]

    else:
        return [street, None]

    return False

def getDistance(p1, p2):
    p1x, p1y = p1
    p2x, p2y = p2

    dist = math.sqrt((p1x-p2x)**2 + (p1y-p2y)**2)
    return dist


def getCoordinates(point):
    return point[0], point[1]

def getSegmentValues(val1, val2, val3, val4, minflg):
    if minflg == True:
        return min(val1,val2), min(val3, val4)
    else:
        return max(val1,val2), max(val3, val4)

def getIntersectingCoordinates(p1,p2,p3,p4,xdiff,ydiff):
    x1, y1 = getCoordinates(p1)
    x2, y2 = getCoordinates(p2)
    x3, y3 = getCoordinates(p3)
    x4, y4 = getCoordinates(p4)

    if x1 == x2 == x3 == x4:
        pnts = [p1,p2,p3,p4]
        insectlist = []
        for pnt in pnts:
            if ydiff[0] <= pnt[1] <= ydiff[1]:
                insectlist.append(pnt)
        return insectlist

    # check equations of lines
    elif x1 != x2 and x3 != x4:
        m1 = (y2-y1)/(x2-x1)
        b1 = y1-m1*x1
        m2 = (y4-y3)/(x4-x3)
        b2 = y3-m2*x3
        # check if ipline equations are equal
        if m1 == m2 and b1 == b2:
            pnts = [p1,p2,p3,p4]
            insectlist = []
            for pnt in pnts:
                if (xdiff[0] <= pnt[0] <= xdiff[1] and
                    ydiff[0] <= pnt[1] <= ydiff[1]):
                    insectlist.append(pnt)
            return insectlist

    detnumX = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
    detnumY = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)

    det = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))

    try:
        xcrd =  detnumX / det
        ycrd = detnumY / det
    except ZeroDivisionError:
        return []

    if (xcrd < xdiff[0] or xcrd > xdiff[1] or
        ycrd < ydiff[0] or ycrd > ydiff[1]):
        return []

    return [(round(xcrd,2), round(ycrd,2))]


def getIntersections(p1, p2, p3, p4):
    x1, y1 = getCoordinates(p1)
    x2, y2 = getCoordinates(p2)
    x3, y3 = getCoordinates(p3)
    x4, y4 = getCoordinates(p4)

    xdiff = (max(getSegmentValues(x1,x2,x3,x4,True)), min(getSegmentValues(x1,x2,x3,x4,False)))
    ydiff = (max(getSegmentValues(y1,y2,y3,y4,True)), min(getSegmentValues(y1,y2,y3,y4,False)))


    return getIntersectingCoordinates(p1,p2,p3,p4,xdiff,ydiff)

def main(args):
    prgobj = Iteration()
    prgobj.cmdloop()

    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))
