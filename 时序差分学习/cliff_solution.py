from cliff_walking import CliffWalkingEnv
import sys
from collections import defaultdict
import itertools
import numpy as np 
from matplotlib import pyplot as plt
import pandas as pd
from matplotlib.font_manager import FontProperties
font_set = FontProperties(fname=r"c://windows//fonts//simsun.ttc", size=15)
env=CliffWalkingEnv()
def make_epsilon_greedy_policy(Q,epsilon,nA):
    def policy_fn(observation):
        A=np.ones(nA,dtype="float")*epsilon/nA
        best_action=np.argmax(Q[observation])   ##从Q表中选出reward最大的动作
        A[best_action]+=(1-epsilon)
        return A
    return policy_fn
def make_twice_epsilon_greedy_policy(Q1,Q2,epsilon,nA):
    def policy_fn(observation):
        A=np.ones(nA,dtype="float")*epsilon/nA
        best_action=np.argmax(Q1[observation]+Q2[observation])   ##从Q表中选出reward最大的动作
        A[best_action]+=(1-epsilon)
        return A
    return policy_fn
def sarsa(env,num_episodes,discount_factor=1.0, alpha=0.5, epsilon=0.1):
    Q=defaultdict(lambda:np.zeros(env.action_space.n))
    R=defaultdict(float)
    policy = make_epsilon_greedy_policy(Q, epsilon, env.action_space.n)
    for num_episode in range(1,num_episodes+1):
        state=env.reset()
        action_probs=policy(state)
        action=np.random.choice(np.arange(len(action_probs)),p=action_probs)
        for t in itertools.count():
            next_state,reward,done,_=env.step(action)
            next_action_probs=policy(next_state)
            next_action=np.random.choice(np.arange(len(next_action_probs)),p=next_action_probs)
            R[num_episode]+=reward
            td_target = reward + discount_factor * Q[next_state][next_action]
            td_delta = td_target - Q[state][action]
            Q[state][action] += alpha * td_delta
            if(done):
                break
            state=next_state
            action=next_action
    return R
def Q_learning(env,num_episodes,discount_factor=1.0, alpha=0.5, epsilon=0.1):
    Q=defaultdict(lambda:np.zeros(env.action_space.n))
    R=defaultdict(float)
    policy = make_epsilon_greedy_policy(Q, epsilon, env.action_space.n)
    for num_episode in range(1,num_episodes+1):
        state=env.reset()
        for t in itertools.count():
            action_probs=policy(state)
            action=np.random.choice(np.arange(len(action_probs)),p=action_probs)
            next_state,reward,done,_=env.step(action)
            R[num_episode]+=reward
            next_action=np.argmax(Q[next_state])
            td_target = reward + discount_factor * Q[next_state][next_action]
            td_delta = td_target - Q[state][action]
            Q[state][action] += alpha * td_delta
            if(done):
                break
            state=next_state
    return R
def twice_learning(env,num_episodes,discount_factor=1.0, alpha=0.5, epsilon=0.1):
    Q1=defaultdict(lambda:np.zeros(env.action_space.n))
    Q2=defaultdict(lambda:np.zeros(env.action_space.n))
    R=defaultdict(float)
    policy=make_twice_epsilon_greedy_policy(Q1,Q2,epsilon,env.action_space.n)
    for num_episode in range(1,num_episodes+1):
        state=env.reset()
        for t in itertools.count():
            action_probs=policy(state)
            action=np.random.choice(np.arange(len(action_probs)),p=action_probs)
            next_state,reward,done,_=env.step(action)
            R[num_episode]+=reward
            s=np.random.rand()
            #print(s)
            if s>0.5:
                best_action=np.argmax(Q1[next_state])
                delta=Q2[next_state][best_action]-Q1[state][action]
                Q1[state][action]+=reward+alpha*delta
            else:
                best_action=np.argmax(Q2[next_state])
                delta=Q1[next_state][best_action]-Q2[state][action]
                Q2[state][action]+=reward+alpha*delta
            if(done):
                break
            state=next_state
    return R
def draw_picture(Rs,Rq,Rt,window_size=50):
    fig2 = plt.figure(figsize=(10,5))
    rewards_smootheds= pd.Series(Rs).rolling(window_size, min_periods=window_size).mean()
    plt.plot(rewards_smootheds,color='g',label='Sarsa') 
    rewards_smoothedq=pd.Series(Rq).rolling(window_size, min_periods=window_size).mean()
    plt.plot(rewards_smoothedq,color='b',label='Q_learning')
    rewards_smoothedt=pd.Series(Rt).rolling(window_size, min_periods=window_size).mean()
    plt.plot(rewards_smoothedt,color='r',label='twice_learning')
    plt.xlabel('幕数',fontproperties=font_set)
    plt.ylabel('每幕的收益和',fontproperties=font_set)
    plt.legend()
    plt.show()
Re=sarsa(env,500)
Rq=Q_learning(env,500)
Rt=twice_learning(env,500)
draw_picture(Re,Rq,Rt)

