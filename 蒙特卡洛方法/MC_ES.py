import numpy as np 
from blackjack import BlackjackEnv
from collections import defaultdict
env=BlackjackEnv()
def mc_prediction(policy,env,numepisodes,disaccount):
    return_sums=defaultdict(float)
    return_counts=defaultdict(float)
    V=defaultdict(float)
    Q=defaultdict(float)
    for i in range(1,numepisodes+1):
        episodes=[]
        state=env.reset() ####重置状态  这是就是随机选取的转态
        action=np.random.randint(0,2)
        next_state,reward,done,_=env.step(action)
        episodes.append([state,action,reward])
        state=next_state
        for t in range(100):
            action=policy[state[0]]
            next_state,reward,done,_=env.step(action)
            episodes.append([state,action,reward])
            if done:
                break
            state=next_state
        state_in_episode=set([tuple(x[0]) for x in episodes])
        G=0
        for state in state_in_episode:
            first_occurence_idx = next(i for i,x in enumerate(episodes) if x[0] == state)
            G = sum([x[2]*(disaccount**i) for i,x in enumerate(episodes[first_occurence_idx:])]) 
            return_sums[(state,policy[state[0]])]+=G
            return_counts[(state,policy[state[0]])]+=1.0
            Q[(state,policy[state[0]])]+=return_sums[(state,policy[state[0]])]/return_counts[(state,policy[state[0]])]
            if(not state[2]):
                if(Q[(state),0]>Q[(state),1]):
                    policy[state[0]]=0
                else:
                    policy[state[0]]=1
    print(policy[:22])
policy=[1]*100
for i in range(20,100):
    policy[i]=0
mc_prediction(policy,env,100000,1.0)


