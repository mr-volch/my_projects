def ticket_cost(N, costs):
    
    if N == 1:
        
        return costs[0][0]
    if N == 2:
        return min(costs[0][1], costs[0][0] + costs[1][0])

    min_costs = [0 for i in range(N)]

    min_costs[0] = costs[-1][0]

    case_one = costs[-2][1]
    case_two = costs[-2][0] + costs[-1][0]
    min_costs[1] = min(case_one, case_two)

    case_one = costs[-3][0] + costs[-2][0] + costs[-1][0]
    case_two = costs[-3][1] + costs[-1][0]
    case_three = costs[-3][0] + costs[-2][1]
    case_four = costs[-3][2]
    min_costs[2] = min(case_one, case_two, case_three, case_four)

    for i in range(3, N):
        min_costs[i] = min(costs[-i-1][0] + min_costs[i-1], costs[-i-1][1] + min_costs[i-2], costs[-i-1][2] + min_costs[i-3])

    return min_costs[-1]
        

N = int(input())
costs = []
for i in range(N):
    costs.append([int(n) for n in input().split()])

print(ticket_cost(N, costs))
