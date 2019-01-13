import time
import random
import pygame
import IPython
import client
import sys
import pandas as pd
import numpy as np

# Set constants for the screen size
FIELD_WIDTH = 50
FIELD_HEIGHT = 35
SQ_SIZE = 20
SCREEN_WIDTH = FIELD_WIDTH*SQ_SIZE
SCREEN_HEIGHT = FIELD_HEIGHT*SQ_SIZE
background = [0,0,0]
RIGHT = 0
UP    = 1
LEFT  = 2
DOWN  = 3

EMPTY = 0
FRUIT = 1
ROCK  = 9
MOVABLE = [EMPTY, FRUIT]

class Snake():
  def __init__(self, env, sid, x=None, y=None, d=RIGHT, l=10):
    self.env = env
    self.d = d
    self.l = l
    self.id = sid;
    self.dead = False
    #self.disappear_in = 4
    self.head_img = pygame.image.load("img/snake_head.png")
    self.body_img = pygame.image.load("img/snake{}.png".format(self.id))
    self.generate_snake(x,y,d,l)
    self.env.snakes.append(self)
    
  def generate_snake(self, x,y,d,l):
    while True:
      if(x is None):
        x = random.randrange(1,FIELD_WIDTH-l-10)
      if(y is None):
        y = random.randrange(1,FIELD_HEIGHT-l-10)
      if(d==RIGHT):
        self.pos = zip(range(x, x+l), [y]*l)
      elif(d==LEFT):
        self.pos = zip(range(x, x+l)[::-1], [y]*l)
      elif(d==UP):
        self.pos = zip([x]*l, range(y, y+l)[::-1])
      else:
        self.pos = zip([x]*l, range(y, y+l))
      
      if self.place_snake_on_map():
        return True
      else:
        x,y = None, None
  
  def place_snake_on_map(self):
    m2 = env.map_filled.copy()
    for i in self.pos:
      if m2.iloc[i[1], i[0]] != 0:
        return False
      m2.iloc[i[1], i[0]] = self.id
    env.map_filled = m2
    return True
  
  def head(self):
    return self.pos[-1]
  
  def direction(self):
    if(self.d==RIGHT):
      return "RIGHT"
    if(self.d==LEFT):
      return "LEFT"
    if(self.d==UP):
      return "UP"
    if(self.d==DOWN):
      return "DOWN"

  def move(self, env):
    if(self.d==RIGHT):
      dx, dy = 1, 0
    elif(self.d==LEFT):
      dx, dy = -1, 0
    elif(self.d==UP):
      dx, dy = 0, -1
    else: # if got anything else, we'll take is as DOWN
      dx, dy = 0, 1
    nx, ny = self.head()[0]+dx, self.head()[1]+dy
    if(nx>=0 and nx<FIELD_WIDTH and ny>=0 and ny<FIELD_HEIGHT
                    and(env.map_filled.iloc[ny,nx] in MOVABLE)):
      self.pos.append([nx,ny])
      if(env.map_filled.iloc[ny,nx] == FRUIT):
        self.l += 1
        env.make_fruit()
      else:
        env.map_filled.iloc[self.pos[0][1], self.pos[0][0]] = EMPTY
        self.pos = self.pos[1:]
      env.map_filled.iloc[ny,nx] = self.id
      
    else:
      #IPython.embed()
      self.dead = True
      self.head_img = pygame.image.load("img/snake_head.png")
      self.body_img = pygame.image.load("img/snake_dead.png")
      
  
  def get_snake(self):
    s = client.SnakeType()
    for i,ss in enumerate(self.pos):
      for xy,sss in enumerate(ss):
        s[i][xy]=self.pos[i][xy]
    return s

class environment():
  def __init__(self, scene=None):
    self.screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
    if(scene is None):
      self.map = pd.DataFrame(np.zeros([FIELD_WIDTH,FIELD_HEIGHT], dtype=np.int16))
    else:
      self.map = pd.read_csv(scene, header=None, index_col=None)
    self.obstacles = zip(self.map.values.nonzero()[1], self.map.values.nonzero()[0])
    self.stone_img = pygame.image.load("img/stone.png")
    self.fruit_img = pygame.image.load("img/cherries.png")
    self.map_filled = self.map.copy()
    self.snakes = []
    self.make_fruit()
    
  def draw(self):
    '''draw the scene'''
    def pos2pix(pos):
      ''' Translates position on the snake board into pixel positions'''  
      x,y = pos
      return pygame.Rect([x*SQ_SIZE, y*SQ_SIZE, (x+1)*SQ_SIZE, (y+1)*SQ_SIZE])
    
    self.screen.fill(background)
    self.screen.blit(self.fruit_img, pos2pix(self.fruit))
    for o in self.obstacles:
      self.screen.blit(self.stone_img, pos2pix(o))
    for s in self.snakes:
      self.screen.blit(s.head_img, pos2pix(s.head()))
      for i in s.pos[:-1]:
        self.screen.blit(s.body_img, pos2pix(i))
    pygame.display.flip()
  
  def get_map(self):
    m = client.MapType()
    for x,mm in enumerate(m):
      for y,mmm in enumerate(mm):
        m[x][y]=self.map_filled.iloc[y,x]
    return m
  
  def make_fruit(self):
    while True:
      x = random.randrange(0,FIELD_WIDTH-1)
      y = random.randrange(0,FIELD_HEIGHT-1)
      if(self.map_filled.iloc[y,x] == EMPTY):
        self.fruit = [x,y]
        self.map_filled.iloc[y,x] = FRUIT
        return


# Open the window. Set the window title and dimensions (width and height)
env = environment(sys.argv[1])

n_snakes = len(sys.argv) -2
snakes = [Snake(env,2+i) for i in range(n_snakes)]
hosts = [h.split(":")[0] for h in sys.argv[2:]]
ports = [int(h.split(":")[1]) for h in sys.argv[2:]]
print hosts, ports

t = 0

while 1:
  for event in pygame.event.get():
    if event.type == pygame.QUIT:
      sys.exit()
  
  env.draw()
  for i,s in enumerate(snakes):
    if not s.dead:
      payload = client.Payload(s.l, env.fruit[0], env.fruit[1], s.get_snake(), env.get_map())
      new_dir = client.get_direction(payload, host=hosts[i], port=ports[i])
      if(new_dir is not None):
        s.d = new_dir
      s.move(env)

  
    
  #IPython.embed()
  time.sleep(0.1)
