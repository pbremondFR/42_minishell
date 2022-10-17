/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 22:25:53 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/10 17:29:46 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 *               ┌─────┐  t_list *  ┌─────────┐  t_list *  ┌──────┐
 * char *line───►│LEXER├─►tok_list─►│PREPARSER├─►tok_list─►│PARSER│
 *               └─────┘            └────┬────┘            └───┬──┘
 *                                       │                     │
 *                                       ▼                     ▼
 *                                  t_list *               t_list *
 *                                  redirs_lst             cmd_list
 *                                       │                     │
 *                                       ▼                     ▼
 *                           ┌─────────────┐               ┌─────────┐
 *       Fully parsed      ◄─┤REDIRS_MAPPER│◄─t_prs_cmd**◄─┤PARSER_TO│
 *  t_prs_cmd *cmd_array[]   └─────────────┘  cmd_array    │2D_ARRAY │
 *                                                         └─────────┘
 */

#ifndef MINISHELL_H
# define MINISHELL_H

// ALLOWED FUNCS: readline, rl_clear_history, rl_on_new_line, rl_replace_line,
// rl_redisplay, add_history, printf, malloc, free, write, access, open, read,
// close, fork, wait, waitpid, wait3, wait4, signal, sigaction, kill, exit,
// getcwd, chdir, stat, lstat, fstat, unlink, execve, dup, dup2, pipe,
// opendir, readdir, closedir, strerror, perror, isatty, ttyname, ttyslot,
// ioctl, getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr,
// tgoto, tputs
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <libft.h>
# include <sys/errno.h>
# include <readline/history.h>
# include <readline/readline.h>

# include "ansi_color.h"

# define WHITESPACES	" \t\n\v\f\r"
# define MS_PROMPT		"\e[1;37mminishell> \e[0m"
# define MS_BASE_PROMPT	"minishell> "

# define F_GLOBAL		0b1
# define F_ENVP			0b10
# define F_TOK_LST		0b100
# define F_CMD_LST		0b1000
# define F_CMD_ARR		0b10000
# define F_REDIR_LST	0b100000
# define F_ALL			0xffffffff

typedef struct s_minishell		t_ms;
typedef struct s_cd_data		t_cd_dat;
typedef enum e_token_types		t_tok_typ;
typedef struct s_lexer_token	t_lex_tok;
typedef struct s_parser_command	t_prs_cmd;

typedef struct s_minishell
{
	t_list		*envp;
	t_list		*tok_list;
	t_list		*redir_list;
	t_list		*cmd_list;
	t_prs_cmd	**cmd_array;
	int			last_code;
}				t_ms;

// The Global Var
# ifndef EXTERN
#  define EXTERN extern
# endif

extern struct s_minishell		*g_ms;

void		ms_perror(const char *s1, const char *s2);
void		ms_init_signals(void);
void		ms_historian(const char *line);
size_t		ms_array_size(const char **a);
void		ms_show_new_prompt(bool nl);

void		debug_print_tok_list(t_list *tok_list, char *msg);
void		ms_tests(t_list *tok_list, t_list *cmd_list, t_prs_cmd **cmd_array);

/* ========================================================================= */
/* ========================================================================= */
// BUILT-INS

// Environment Variable
char		*expand_env(const char *s);
char		*ms_find_env(char	*target);
char		*expand_env_get_env(const char *s, size_t i);
size_t		expand_env_get_size(const char *s);

// Builtin env
int			ms_env_checker(char *s);
int			env(void);

// Builtin export
int			ms_export_builtin(char *argv[]);
int			ms_export(char *s);
bool		ms_export_is_format_valid(const char *s, const char *prgm_name);

// Builtin unset
int			ms_unset(char *t);
void		unset_remove_one(t_list **crnt_ptr, t_list *last);
int			verify_env_name(char *t);

// Builtin echo
int			ms_echo(char **argv);

// Builtin pwd
int			ft_pwd(void);

// Builtin cd
typedef struct s_cd_data
{
	char	*old_cwd;
	char	*new_cwd;
	char	*home;
}				t_cd_dat;

int			ft_cd(const char *path);
int			ft_exec_cd(const char **argv);
int			update_pwd(void);
void		update_oldpwd(const char *value);

// Builtin exit
void		ms_exit(int code, int free_oflags, int verbose);
void		ms_exec_exit(t_prs_cmd *cmd);

/* ========================================================================= */
/* ========================================================================= */
// $PATH and EXECS

char		**get_paths(void);
void		free_array(char **array);
char		*find_exec(char *command);
int			exec_command(char *path, char *args[]);
char		**convert_envp(void);
char		*get_path_with_tilde(char *home, char *path, size_t home_len);

/* ========================================================================= */
/* ========================================================================= */
// GLOBAL VARIABLE

void		free_global_var(int o_flags);
void		ms_init_global_var(const char **envp);

/* ========================================================================= */
/* ========================================================================= */
// LEXER

# define TOK_ENUM_SIZ	T_STRING

typedef enum e_token_types
{
	T_UNDEF,
	T_SEP,
	T_BLANK,
	T_DQUOTE,
	T_SQUOTE,
	T_SLESS,
	T_DLESS,
	T_SGREAT,
	T_DGREAT,
	T_PIPE,
	T_SEMI,
	T_ALNUM,
	T_CMD,
	T_FILE,
	T_SSTRING,
	T_DSTRING,
	T_STRING
}			t_tok_typ;

typedef struct s_lexer_token
{
	t_tok_typ	type;
	char		*value;
}				t_lex_tok;

// lexer.c
t_list		*ms_lexer(const char *line, size_t len);
t_lex_tok	*ms_lex_get_token(char *line, size_t *cursor);
t_tok_typ	ms_lex_get_tok_type(const char *word);

// lexer_aux.c
char		*ms_lex_xtract_word_str(char *line, size_t *cursor,
				t_tok_typ *tok_type);
char		*ms_lex_xtract_word_sep(char *line, size_t *cursor,
				t_tok_typ *tok_type);

// lexer_utils.c
int			is_char_lex_sep(char c);
int			is_tok_lex_sep(t_tok_typ type);
t_tok_typ	lex_char_check(char c);
t_lex_tok	*ms_lex_create_token(char *word, t_tok_typ type);
void		ms_del_token(t_lex_tok **token_ptr);

// lexer_utils_2.c
void		ms_free_token(void *token);

// quote_checking.c
int			ms_lex_check_quote_parity(char const *line, size_t len);

/* ========================================================================= */
/* ========================================================================= */
// PARSER

// pre_parser.c
t_list		*ms_preparser(t_list **tok_strm);
t_list		*ms_preparser_2nd_pass(t_list **p_redir_list);

// token_padding.c
void		ms_token_padding(t_list **p_tok_list, t_list **p_redir_list);

typedef struct s_parser_command
{
	t_lex_tok	*token;
	char		**argv;
	char		*bin_path;
	int			id;
	t_tok_typ	type;
	int			pipe_fd[2];
	int			fd_in;
	int			fd_out;
	pid_t		pid;
	int			code;
	t_lex_tok	**redir_array;
	int			package;
}				t_prs_cmd;

char		**ms_prs_create_argv(t_list *tok_stream, char *name);
int			ms_prs_argv_size(t_list *tok_stream);
int			ms_prs_is_sep(t_tok_typ type);
t_list		*ms_parser(t_list *tok_strm);
char		*ms_prs_get_bin_path(t_prs_cmd *cmd, char *prgm_path);

// string_parsing.c
char		*ms_prs_parse_word(char *word);

// env_expanding*.c
size_t		ms_prs_get_exp_strlen(const char *word);
char		*ms_prs_exp_str_env(const char *word, size_t word_len);
char		*ms_prs_get_envar_name(const char *str);
bool		ms_prs_in_envar_name(const char *s, size_t i);

// tilde_expanding.c
char		*ms_prs_exp_tilde(const char *word, size_t word_len);

// parser_utils.c
int			is_sgreat_dgreat(t_tok_typ type);
int			is_sless_dless(t_tok_typ type);
void		ms_free_cmd(void *command);
t_prs_cmd	*ms_prs_create_cmd(t_lex_tok *token, t_list *tok_strm, int id);
t_prs_cmd	**ms_prs_lst_2darray(t_list *cmd_lst);

// redirs_mapping.c
void		ms_prs_map_redirs_to_cmd(t_list *redir_list, t_prs_cmd **cmds);

/* ========================================================================= */
/* ========================================================================= */
// Redirections

// LOGIC
int			ms_cmd_has_redirects(t_prs_cmd *cmd);
int			ms_cmd_pipe_status(t_prs_cmd **cmds, size_t i, int package);
char		*ms_heredoc_logic(t_prs_cmd **cmds, int target, int i);

// SEARCH
int			ms_find_redirections(t_prs_cmd **cmds);
int			ms_handle_redirections(t_prs_cmd **cmds, struct termios term);
void		ms_find_begining_chain(t_prs_cmd **cmds, size_t i);
void		ms_find_and_run_commands(t_prs_cmd **cmds);
void		ms_error_with_cmd(t_prs_cmd **cmds, int i, int enumb);
int			ms_find_last_stdin_in_chain(t_prs_cmd *cmd);

// EXECUTION
void		ms_fill_redirect_array(t_prs_cmd **cmds, t_list *redirections);
void		ms_fork_plumber(t_prs_cmd **cmds, size_t i, int type);
int			ms_stdout_redirect_handle(t_prs_cmd **cmds, int i, int start);
int			ms_stdin_redirect_handle(t_prs_cmd **cmds, int i, int start);
void		ms_heredoc_handle(t_prs_cmd **cmds, int target, int i);
void		ms_prepare_execution(t_prs_cmd **cmds, size_t i, int type);
void		ms_prepare_bulitin_exec(t_prs_cmd **cmds, size_t i, int type);
void		ms_redirect_stdout(t_prs_cmd **cmds, size_t i);
void		ms_redirect_stdin(t_prs_cmd **cmds, int i);
void		ms_sig_forked_handler(int signal);
void		ms_sig_faker_handler(int sig);

// ENDING
void		ms_close_pipes(t_prs_cmd **cmds);
int			ms_get_return_code(t_prs_cmd **cmds);

// DUMB
void		ms_only_redirects(t_list *redir_list);
void		ms_setup_redirects(t_prs_cmd **cmds, size_t i, int type);
void		ms_faker_heredoc(char *value);

// ENUM
typedef enum e_redirection_types
{
	C_SOLO		= 0b000000001,
	C_EXIT		= 0b000000010,
	C_BRIDGE	= 0b000000100,
	C_ENTER		= 0b000001000,
	R_OUT_S		= 0b000010000,
	R_OUT_D		= 0b000100000,
	R_IN		= 0b001000000,
	R_HERE		= 0b010000000
}			t_redi_typ;

/* ========================================================================= */
/* ========================================================================= */
// PROGRAM EXECUTION

int			ms_is_builtin(t_prs_cmd *cmd);
int			ms_exec_builtin(t_prs_cmd *cmd);
void		ms_exec_prgm(const char *pathname, char *argv[]);

/* ========================================================================= */
/* ========================================================================= */
// SMALL BONUSES

char		*ms_prompt(void);

#endif
