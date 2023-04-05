#include "minishell.h"

static char	*get_abs_path(char *path, char *cmd)
{
	char	*cmd_w_path;
	char	*tmp;

	tmp = ft_strjoin(path, "/");
	if (tmp == NULL)
	{
		free(tmp);
		return (NULL);
	}
	cmd_w_path = ft_strjoin(tmp, cmd);
	if (cmd_w_path == NULL)
	{
		free(tmp);
		free(cmd_w_path);
		return (NULL);
	}
	free(tmp);
	return (cmd_w_path);
}

static int	init_paths(char ***paths)
{
	char	*path;

	path = get_env("PATH");
	if (path == NULL)
		return (EXIT_FAILURE);
	*paths = ft_split(path, ':');
	free(path);
	if (*path_splitted == NULL)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static char	*find_command(char *cmd)
{
	char	**paths;
	char	*cmd_w_path;
	int		i;

	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	if (init_paths(&paths) == EXIT_FAILURE)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		cmd_w_path = get_abs_path(paths[i], cmd);
		if (cmd_w_path == NULL)
			return (NULL);
		if (access(cmd_w_path, F_OK) == 0)
		{
			ft_free_split();
			return (cmd_w_path);
		}
		free(cmd_w_path);
		i++;
	}
	ft_free_split();
	return (NULL);
}

int	execute_cmd(char *cmd_w_paths, Node *commands)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
		return (EXIT_FAILURE);
	else if (!pid)
		return (execute_child());
	waitpid(pid, &status, 0);
}

void	executer(Node *commands)
{
	char	*cmd_w_path;
	int		exit_status;

	/*Burda fdler açılacak döngünün içi bayağı değişecek. pipexi almayı unutmusum 
	evden keygenlede uğraşmak istemedim :)) args - 1 kadar pipe çalışacak. redirectler yapılacak
	heredoc için mantıklı bir şey bulmak zor */
	while (commands)
	{
		if (is_builtin(commands->args[0]))
			execute_builtin(commands);
		else
		{
			cmd_w_path = find_command(commands->args[0]);
			exit_status = execute_cmd(cmd_w_path, commands);
		}
		commands = commands->next;
	}
	return (exit_status);
}
