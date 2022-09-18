/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfranca- <lfranca-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/27 18:11:58 by lfranca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/lib/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stdio.h>
# include <unistd.h>
# include <signal.h>
# include <stddef.h>
# include <stdlib.h>

# define TYPE_PIPE 1
# define TYPE_NULL 0

typedef struct s_redir
{
	char	**argv;
	char	**type;
	char	**commands;
	int		argc;
	int		has_in;
	int		has_out;
	int		has_heredoc;
	char	*filename_failed;
}	t_redir;

typedef struct s_cmd
{
	struct s_cmd	*next;	
	char			**tokens;
	int				type_cmd;
	int				pre_flag;
	int				has_redir;
	int				fd[2];
}	t_cmd;

typedef struct s_variables
{
	struct s_variables	*next;
	char				*value;
	char				*name;		
}	t_variables;

typedef struct s_global
{
	t_variables	*head_env;
	t_variables	*head_local;
	int			qty_cmd;
	t_cmd		*head;
	int			exit_status;
	char		*delimiter;
}	t_global;

extern t_global	g_global;

void		ft_signal_interrupt(int	*ret, t_redir *redir, int *saved_stds);
void		ft_unlink_heredoc(t_redir *redir);
int			ft_no_path_cmd(char *path, char *cmd);
void		ft_error_cmd_not_found(char *cmd);
void		ft_print_error_filename(char *filename);
void		eu_open_pipe(t_cmd *current_cmd, t_cmd *next_cmd);
int			ft_is_strcmp_true(char *name_list, char *name_input);
int			unset_delete_var(t_variables **head, char *name);
void		eh_free_child_heredoc(t_redir *redir, char **env_matrix);
int			eh_read_heredoc(t_redir *redir, char **env_matrix, int *tmp_fd);
int			pd_is_between_double(char **tokens, int i);
int			pd_found_closed_quote(char **tokens, int i);
void		se_set_exec_signals(void);
void		se_set_main_signals(void);
void		se_interrupt_process(int signal);
void		se_quit_process(int signal);
void		ev_change_value(t_variables *tmp, char **content_var);
void		ef_treat_exit(t_cmd *current_cmd, int *saved_std, char **env);
int			ev_declare_var(char **tokens);
int			ev_is_single_cmd_var_declare(t_cmd *current_cmd);
void		er_exec_redir(t_cmd *current_cmd, t_redir *redir, char **env);
int			unset_remove_var(char **tokens, int index);
void		ft_unset(char **tokens);
void		ft_pwd(void);
void		export_creat_var(char **tokens, int i, t_variables *tmp);
void		ft_export(char **tokens);
int			exit_digit_to_str(char *str);
void		exit_status_one(void);
void		ft_exit_message(void);
void		ft_exit(t_cmd *node, char **tokens);
void		ft_env(char **cmd);
void		ft_echo(char **tokens);
int			cd_error_excess_args(char **tokens);
int			cd_home(char **tokens);
void		cd_old_pwd(void);
int			cd_print_error1(char **tokens);
void		cd_missing_dir(char **tokens);
void		ft_cd(char **tks);
int			ec_exec_builtin(t_cmd *current_cmd, char **cmd);
int			ec_is_builtin(char **tokens);
int			er_prep_stdin_redir(t_cmd *cmd, t_redir *redir, int tmp_fd);
int			eh_herepipe(t_cmd *cmd, t_redir *redir, int saved_out, char **env);
int			eh_exec_here(t_cmd *cmd, t_redir *redir, int saved_out, char **env);
int			sr_open_unnecessary_file(t_redir *redir);
int			sr_parse_redir(t_redir *redir, char *cmdline);
void		sr_init_redir(t_redir *redir, char *cmdline);
char		*sr_one_line(char **tokens);
void		pc_prepare_node(t_cmd *temp);
int			er_prepare_stdin(t_cmd *current_cmd, t_redir *redir);
int			er_prepare_stdout(t_cmd *current_cmd, t_redir *redir);
int			er_do_cmd(t_cmd *cur_cmd, char **cmd, t_redir *redir, char **env);
int			ep_exec_child(t_cmd *cmd, t_cmd *next_cmd, char **env, char *path);
void		ep_set_fd_pipe(t_cmd *current_cmd, t_cmd *next_cmd);
int			ep_builtin_pipe(t_cmd *current_cmd, char **env_matrix, char *path);
int			eh_create_tmp_file(void);
void		eh_interrupt_heredoc(int signal);
char		**el_env_to_matrix(void);
int			el_counter_envs(void);
void		er_exec_no_command(t_cmd *cmd, t_redir *redir, char **env);
void		er_free_signal_interrupt(t_redir *redir, int *saved_stds);
void		ft_print_error_filename(char *filename);
void		er_free_final_redir(t_redir *redir, int *saved_stds, char **env);
int			er_redir_child(t_cmd *cmd, t_redir *redir, char **env);
int			er_no_pipesheredoc(t_redir *redir, t_cmd *cmd, char **env);
int			er_rednpipe(t_cmd *cmd, t_redir *redir, int *saved_std, char **env);
char		*eu_find_cmdpath(char *line_paths, char *cmd);
void		dc_close_stds(int *saved_stds);
void		ef_finalize_exec_process(int ret, int *saved_stds);
int			ec_exec_pipe(t_cmd *current_cmd, char **env_matrix);
int			ec_is_builtin(char **tokens);
int			ec_no_redir_exec(t_cmd *current_cmd, int *saved_stds);
void		ef_free_envs(char **env_matrix);
void		dc_dup_stds(int *saved_stds);
int			eu_is_here_interrupted(int ret, t_redir *redir, char **env_matrix);
int			er_redpiped(t_cmd *cmd, t_redir *redir, int *saved_std, char **env);
void		ef_free_redir(t_redir *redir);
void		ef_free_redir_envs(t_redir *redir, char **env_matrix);
void		er_treat_redir_no_cmd(t_cmd *current_cmd);
int			eu_is_cmd_piped(t_cmd *current_cmd);
void		sr_set_index_valid_file(t_redir *redir);
void		sr_error_open_file(char *filename);
int			sr_open_unnecessary_file(t_redir *redir);
char		*sr_set_type_redir(char *cmdline, int i);
int			sr_parse_redir(t_redir *redir, char *cmdline);
void		sr_init_redir(t_redir *redir, char *cmdline);
char		*sr_one_line(char **tokens);
t_redir		*sr_treat_redir(t_cmd *current_cmd, int *ret);
int			er_execute_redires(t_cmd *current_cmd, int *saved_stds);
int			ev_is_valid_declare(t_cmd *current_cmd);
int			ec_exec_process(t_cmd *head, int *saved_stds);
void		ma_decide_exec(int ret);
void		rq_remove_quotes(char **token);
int			pu_has_redir(char **tokens);
int			ps_jump_var_declaration(char **tokens);
char		**ps_remove_start_var_declares(char **tokens);
int			pu_is_var_declare_followed_by_redir(char **tokens);
int			pr_is_token_redir(char *token);
char		**pr_order_cmd(char **tokens, int *total_tokens);
int			pu_are_tokens_declarations(char **tokens);
int			pc_print_error(char *unexpected_token);
int			sv_sintaxe_verification(char **tokens, int counter_nodes);
int			ps_validate_sintaxe(t_cmd *temp, int counter_nodes);
t_variables	*el_search_var(t_variables *head, char *name);
char		*pr_join_rest(char **tokens, int *index, char *join, int question);
void		pr_rebuild_string(char **tokens, char *value, int *index_dolar);
int			pv_extract_varname(char **tokens, int *i);
int			pl_verify_expand(char **tokens);
char		*pl_rebuild_spaced(int index, char *subcommand, int start);
void		pl_insert_spaces(char **subcommand, int *index);
int			ps_valid_redir(char **subcommand);
int			pd_is_token_redir(char *token);
t_cmd		*pn_create_node(void);
int			pn_create_list(char **subcommands);
int			pc_count_pipes(char **subcommands, int qty_pipes);
void		su_found_fragment(int *word_ln, char const *cmdline, int *j);
void		su_found_len_word(int *word_ln, char *cmdline, int *j);
void		su_verify_quote(int *i, char *cmdline, char placeholder_quote);
char		**sm_split_3(char *cmdline, char delimiter, int *qty_pipes);
int			pc_management_parse(char *cmdline);
void		fl_free_var_list(t_variables **head);
void		fl_free_list(void);
void		ma_clear_ctrld(void);
void		mm_main_loop(int ret);
void		er_free_matrix(char **matrix);
t_variables	*el_var_node(char **content_var);
void		mm_list_environ(char **environ);
void		se_handler(int signal);
int			main(int argc, char **argv, char **environ);

#endif
