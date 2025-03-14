/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzurita <dzurita@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 16:38:37 by dzurita           #+#    #+#             */
/*   Updated: 2024/06/13 11:24:15 by dzurita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <ctype.h>
# include <stdbool.h>
# include <fcntl.h>
# include <sys/wait.h>

# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h> 
# include <termios.h>  

# define DELIMITER "|<>\"\'$ '\t'"
# define STDIN 0
# define STDOUT 1
# define STDERR 2
# define TRUE 1
# define FALSE 0

typedef int	t_bool;
extern int	g_num;

typedef struct s_split
{
	char	**str;
	size_t	start;
	size_t	end;
	size_t	len;
	size_t	i;
}	t_split;

typedef enum s_token_name
{
	TOKEN_PIPE,
	TOKEN_STR,
	TOKEN_DOLAR,
	TOKEN_SPACE,
	TOKEN_HEREDOC,
	TOKEN_IN_REDIRECT,
	TOKEN_OUT_REDIRECT,
	TOKEN_REDIR_APPEND,
	TOKEN_S_QUOTE,
	TOKEN_DQUOTE_OPEN,
	TOKEN_DQUOTE_CLOSED,
	TOKEN_ERROR,
	TOKEN_EOL,
}	t_token_name;

typedef struct s_execute
{
	pid_t	*child;
	int		*pipesfd;
	int		fdtrack;
	int		currentfd;
}	t_execute;

typedef struct s_lexer
{
	char	*start;
	size_t	len;
}	t_lexer;

typedef struct s_token
{
	t_token_name	type;
	t_lexer			pos;
}	t_token;

typedef struct s_char_iter
{
	char	*start;
	char	*end;
	int		d_flag;
}	t_char_iter;

typedef struct s_parse
{
	char	**cmd;
	char	**rec_file;
	t_bool	outfile;
	t_bool	infile;
}	t_parse;

typedef struct s_data
{
	struct termios	old;
	struct termios	new;
	char			**env;
	char			**exp;
	char			*str_rl;
	char			*root;
	int				str;
	int				irec;
	int				i_token;
	int				i_pipex;
	int				i_quate;
	int				i_str;
	int				i_parse;
	int				shlvl;
	int				exit_status;
	int				here_doc_fd;
	int				stdin_backup;
	int				stdout_backup;
	bool			is_exp;
	t_token			*lexer_array;
	t_parse			*parse;
	t_execute		*exec;
}	t_data;
//lexer
void		lexer_tokenizer(t_data *data);
void		take_error(t_token *token, t_char_iter *iter);
int			ft_realloc(t_token **token, size_t size);
char		*char_find_dq(t_char_iter *self);
void		take_redir_append(t_char_iter *iter, t_token *token);
void		add_token(t_token *token, t_char_iter *iter);
int			input_check(t_data *content);
void		take_redir_in(t_token *token, t_char_iter *iter);
void		take_redir_out(t_token *token, t_char_iter *iter);
void		take_s_quote(t_token *token, t_char_iter *iter);
void		take_d_quote(t_token *token, t_char_iter *iter);
void		take_exp(t_token *token, t_char_iter *iter);
void		take_just_dollar(t_token *token, t_char_iter *iter);
void		take_dollar(t_token *token, t_char_iter *iter);
void		take_string(t_token *token, t_char_iter *iter);
//parse
void		creating_parse(t_data *data);
void		add_data_to_parse(t_parse *parse, t_data *data, int i_pipex);
char		*ft_add_cmd_str(const char *src, int len, t_data *data);
void		parse_dolar(t_data *data, t_parse *parse, int i_token, int i_parse);
int			index_after_quate(t_data *data, int i);
char		*make_str_dquote(t_data *data, int i_token, int i_quate);
void		parse_str(t_data *data, t_parse *parse, int i_parse);
bool		is_redic(t_data *data, int i);
bool		check_token_str_dquote(t_data *data, int i_token);
char		*check_dquote_str(t_data *data, int i_token);
bool		parse_dolar_envp(t_data *data, t_parse *parse, char *str,
				int i_token);
void		count_str_redic(t_data *data);
void		parse_redic(t_data *data, t_parse *parse, int i_parse);
void		current_itoken(t_data *data);
int			ft_cont_str(const char *s, char c);
char		*parse_dolar_dquate(t_data *data, int i_token);
char		*str_redc_dolar(t_data *data, int i_token);
char		*check_str_envp_redc(t_data *data, char *str, int i_token);
void		free_struct_parse(t_data *data);
int			parse_str_loop(t_data *data, t_parse *parse, int i_parse, int i);
char		*make_recd_str(t_data *data, int i_token);
bool		check_error_token(t_data *data);
void		pipex_parse_error(t_data *data, int i);

//signals
void		rl_replace_line(const char *text, int clear_undo);
void		receive_signal(int val);
char		*tcsetreadline(t_data *msh, int n);
void		load_termios(t_data *msh);
void		check_signal(t_data *content);

//iter funtions
t_char_iter	char_iter_constructor(char *start, size_t	len);
char		*char_iter_cursor(t_char_iter *self);
char		char_iter_peek_next(t_char_iter *self);
char		*char_find_dq(t_char_iter *self);
char		char_iter_peek(t_char_iter *self);
char		char_iter_next(t_char_iter *self);
int			ft_realloc(t_token **token, size_t size);

//execution
char		**export(char *arg, char **table, t_data *content);
void		env(t_data *content);
void		build_export(t_data *content);
void		pre_export(t_data *cnt);
void		initialize_export(t_data *content, char *arg);
void		run_builtin(t_data *cnt);
void		echo(char **args, t_data *content);
void		built_exit(char **args, t_data *cnt);
char		**add_space(char **table, int linel, t_data *content);
void		unset_variable(t_data *content, char **args);
void		execution(t_data *cnt, int i, int fd);
void		executor(t_data *cnt);
void		exec(char **cmd, char **env);
t_bool		check_built_in(char **args);
void		piping_and_forking(t_data *cnt, int i);
t_execute	*init_exec_struct(int pipes);
char		*get_path(char *cmd, char **envp, int *p);
void		run_builtin_child(char **args, t_data *cnt);
int			redirect(t_data	*cnt, int i);
void		prepare_exit(t_data *content, int status);
void		here_doc(char *limiter, t_data *cnt);
void		check_here_doc(t_data *cnt);
void		print_export(t_data *content);
void		print_pwd(t_data *cnt);
void		change_directory(char *path, t_data *content);

//utils
char		*safe_strjoin(char const *s1, char const *s2, t_data *content);
char		*safe_strdup(const char *src, t_data *content);
void		*safe_calloc(size_t nitems, size_t size, t_data *content);
void		free_array(char **args);
int			checkpath(char *path);
void		create_envp(char **env, t_data *content);
char		*safe_substr(char const *src, unsigned int start, size_t len,
				t_data *content);

#endif
