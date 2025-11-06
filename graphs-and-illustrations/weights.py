import numpy as np
import matplotlib.pyplot as plt

paramValues = np.zeros((200,200))

points = [
    ((20, 20), 0.6),
    ((100, 150), 0),
    ((110, 140), 1)
]

distances = np.zeros(len(points))
weights = np.zeros(len(points))
vals = np.zeros(len(points))

for i in range(200):
    for j in range(200):
        for k in range(len(points)):
            distances[k] = (points[k][0][0] - i) ** 2 + (points[k][0][1] - j) ** 2
        if min(distances) < 1:
            for k in range(len(points)):
                weights[k] = 1 if distances[k] < 1 else 0
        else:
            weights = 1 / distances
        weightsSum = np.sum(weights)
        weights /= weightsSum
        vals *= 0
        for k in range(len(points)):
            vals[k] += weights[k] * points[k][1]
        paramValues[i,j] = np.sum(vals)

plt.imshow(paramValues % 1 < 0.5)


for p in points:
    plt.scatter(p[0][1], p[0][0], c='k')
plt.show()

